#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

int main()
{
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "3D Engine", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Configure global OpenGL state
    glEnable(GL_DEPTH_TEST);

    // Create and compile shaders
    Shader shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");
    Shader shadowShader("assets/shaders/shadow_map.vert", "assets/shaders/shadow_map.frag");

    // Load 3D model
    Model ourModel("assets/models/backpack/backpack.obj");

    // Initialize shadow map
    ShadowMap shadowMap(1024, 1024);

    // Set up view and projection matrices
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));

    // Light position
    glm::vec3 lightPos(2.0f, 4.0f, -1.0f);

    // Render loop
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        // 1. First render to depth map
        shadowMap.bindFramebuffer();
        glClear(GL_DEPTH_BUFFER_BIT);

        shadowShader.use();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
        shadowShader.setMat4("lightSpaceMatrix", shadowMap.getLightSpaceMatrix());
        shadowShader.setMat4("model", model);

        ourModel.Draw(shadowShader);

        shadowMap.unbindFramebuffer();

        // 2. Then render scene as normal
        glViewport(0, 0, 800, 600);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        shader.setMat4("model", model);
        shader.setMat4("lightSpaceMatrix", shadowMap.getLightSpaceMatrix());
        shader.setVec3("lightPos", lightPos);
        shader.setVec3("viewPos", glm::vec3(0.0f, 0.0f, -3.0f));

        // Bind shadow map
        shadowMap.bindDepthMap(1);
        shader.setInt("shadowMap", 1);

        ourModel.Draw(shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
