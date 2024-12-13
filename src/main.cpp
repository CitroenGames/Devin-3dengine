#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>

#include "core/shader.hpp"
#include "core/model.hpp"
#include "rendering/shadow_map.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main(int argc, char* argv[])
{
    bool test_mode = (argc > 1 && std::string(argv[1]) == "--test");
    GLFWwindow* window = nullptr;

    // Get executable path and set asset paths
    std::filesystem::path execPath = std::filesystem::canonical("/proc/self/exe").parent_path().parent_path();
    std::string shaderPath = (execPath / "assets/shaders").string();
    std::string modelPath = (execPath / "assets/models").string();

    std::cout << "Initializing 3D Engine..." << std::endl;

    // Initialize GLFW in regular mode only
    if (!test_mode) {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        window = glfwCreateWindow(800, 600, "3D Engine", NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return -1;
        }

        glEnable(GL_DEPTH_TEST);
    }

    try {
        std::cout << "Testing core components..." << std::endl;

        // Test shader compilation
        std::cout << "Testing shader compilation..." << std::endl;
        bool shader_test = true;
        Shader* shader = nullptr;
        Shader* shadowShader = nullptr;
        try {
            shader = new Shader((shaderPath + "/basic.vert").c_str(), (shaderPath + "/basic.frag").c_str(), test_mode);
            shadowShader = new Shader((shaderPath + "/shadow_map.vert").c_str(), (shaderPath + "/shadow_map.frag").c_str(), test_mode);
            std::cout << "✓ Shader compilation successful" << std::endl;
        } catch (const std::exception& e) {
            std::cout << "✗ Shader compilation failed: " << e.what() << std::endl;
            shader_test = false;
        }

        // Test model loading
        std::cout << "Testing model loading..." << std::endl;
        bool model_test = true;
        Model* ourModel = nullptr;
        try {
            ourModel = new Model((modelPath + "/backpack/backpack.obj").c_str(), test_mode);
            std::cout << "✓ Model loading successful" << std::endl;
        } catch (const std::exception& e) {
            std::cout << "✗ Model loading failed: " << e.what() << std::endl;
            model_test = false;
        }

        if (test_mode) {
            if (shader_test && model_test) {
                std::cout << "\nAll core components tested successfully!" << std::endl;
                delete shader;
                delete shadowShader;
                delete ourModel;
                return 0;
            } else {
                std::cout << "\nSome tests failed!" << std::endl;
                delete shader;
                delete shadowShader;
                delete ourModel;
                return 1;
            }
        }

        // Regular rendering mode
        ShadowMap shadowMap(1024, 1024);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
        glm::vec3 lightPos(2.0f, 4.0f, -1.0f);

        while(!glfwWindowShouldClose(window))
        {
            processInput(window);

            shadowMap.bindFramebuffer();
            glClear(GL_DEPTH_BUFFER_BIT);

            shadowShader->use();
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
            shadowShader->setMat4("lightSpaceMatrix", shadowMap.getLightSpaceMatrix());
            shadowShader->setMat4("model", model);

            ourModel->Draw(*shadowShader);

            shadowMap.unbindFramebuffer();

            glViewport(0, 0, 800, 600);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            shader->use();
            shader->setMat4("projection", projection);
            shader->setMat4("view", view);
            shader->setMat4("model", model);
            shader->setMat4("lightSpaceMatrix", shadowMap.getLightSpaceMatrix());
            shader->setVec3("lightPos", lightPos);
            shader->setVec3("viewPos", glm::vec3(0.0f, 0.0f, -3.0f));

            shadowMap.bindDepthMap(1);
            shader->setInt("shadowMap", 1);

            ourModel->Draw(*shader);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        delete shader;
        delete shadowShader;
        delete ourModel;

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        if (!test_mode) {
            glfwTerminate();
        }
        return -1;
    }

    if (!test_mode) {
        glfwTerminate();
    }
    return 0;
}
