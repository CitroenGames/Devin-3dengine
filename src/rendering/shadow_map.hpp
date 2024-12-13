#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "../core/shader.hpp"

class ShadowMap {
public:
    ShadowMap(unsigned int width = 1024, unsigned int height = 1024);
    ~ShadowMap();

    void bindFramebuffer();
    void unbindFramebuffer();
    void bindDepthMap(unsigned int textureUnit);
    glm::mat4 getLightSpaceMatrix();

private:
    unsigned int depthMapFBO;
    unsigned int depthMap;
    unsigned int SHADOW_WIDTH, SHADOW_HEIGHT;
    glm::mat4 lightSpaceMatrix;

    void setupShadowMap();
    void calculateLightSpaceMatrix();
};
