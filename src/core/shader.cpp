#include "shader.hpp"
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

bool Shader::validateShaderFile(const char* path) {
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        shaderFile.open(path);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        return true;
    }
    catch(std::ifstream::failure& e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << path << std::endl;
        return false;
    }
}

Shader::Shader(const char* vertexPath, const char* fragmentPath, bool test_mode) : test_mode(test_mode) {
    if (test_mode) {
        if (!validateShaderFile(vertexPath) || !validateShaderFile(fragmentPath)) {
            throw std::runtime_error("Failed to validate shader files");
        }
        ID = 0;
        return;
    }

    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch(std::ifstream::failure& e) {
        throw std::runtime_error("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ");
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertex, fragment;
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() {
    if (!test_mode) {
        glUseProgram(ID);
    }
}

void Shader::setBool(const std::string &name, bool value) const {
    if (!test_mode) {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
}

void Shader::setInt(const std::string &name, int value) const {
    if (!test_mode) {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
}

void Shader::setFloat(const std::string &name, float value) const {
    if (!test_mode) {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
    if (!test_mode) {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
    if (!test_mode) {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }
}

int Shader::getUniformLocation(const std::string &name) {
    if (test_mode) return -1;

    if (uniformLocationCache.find(name) != uniformLocationCache.end())
        return uniformLocationCache[name];

    int location = glGetUniformLocation(ID, name.c_str());
    uniformLocationCache[name] = location;
    return location;
}

void Shader::checkCompileErrors(unsigned int shader, std::string type) {
    if (test_mode) return;

    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            throw std::runtime_error("ERROR::SHADER_COMPILATION_ERROR of type: " + type + "\n" + infoLog);
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            throw std::runtime_error("ERROR::PROGRAM_LINKING_ERROR of type: " + type + "\n" + infoLog);
        }
    }
}

Shader::~Shader() {
    if (!test_mode && ID != 0) {
        glDeleteProgram(ID);
    }
}
