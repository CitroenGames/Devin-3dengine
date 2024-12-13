#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

class Shader {
public:
    Shader(const char* vertexPath, const char* fragmentPath, bool test_mode = false);
    ~Shader();

    void use();
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;

private:
    unsigned int ID;
    std::unordered_map<std::string, int> uniformLocationCache;
    bool test_mode;

    void checkCompileErrors(unsigned int shader, std::string type);
    int getUniformLocation(const std::string &name);
    bool validateShaderFile(const char* path);
};
