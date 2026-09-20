#pragma once

#include <iostream>
#include <vector>
#include "glad/gl.h"
#include "glm/gtc/matrix_transform.hpp"
#include "shader.h"
#include "scene.h"

// default params
const glm::vec3 POSITION = glm::vec3(0.0f);
const float NEAR = 0.1f;
const float FAR = 100.0f;

class PointLight
{
public:
    glm::vec3 position;
    glm::vec3 color;
    unsigned int depthCubeMap;
    unsigned int fbo;

    // shadow properties
    float near;
    float far;
    std::vector<glm::mat4> shadowTransformMatrices;

    PointLight(glm::vec3 position = POSITION, float near = NEAR, float far = FAR, glm::vec3 color = glm::vec3(1.0f))
    {
        this->position = position;
        this->color = color;
        this->near = near;
        this->far = far;

        this->setup();
    }

    void renderShadow(Shader shader, Scene scene)
    {
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
        glClear(GL_DEPTH_BUFFER_BIT);
        shader.use();
        // shader.uniformMat4("shadowMatrix", this->shadowTransformMatrices[0]);
        for (int i = 0; i < 6; ++i)
        {
            shader.uniformMat4(("shadowTransformMatrices[" + std::to_string(i) + "]").c_str(), this->shadowTransformMatrices[i]);
        }
        shader.setFloat("farPlane", this->far);
        scene.render(shader);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

private:
    glm::mat4 projectionMatrix;

    void setup()
    {
        glGenFramebuffers(1, &this->fbo);

        glGenTextures(1, &this->depthCubeMap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, this->depthCubeMap);
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->depthCubeMap, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // setting up projection matrix
        this->projectionMatrix = glm::perspective(glm::radians(90.0f), static_cast<float>(SHADOW_WIDTH) / SHADOW_HEIGHT, this->near, this->far);

        // creating shadow transform matrices
        for (int face = 0; face < 6; ++face)
        {
            // left (why did book use -1 for up vector)
            this->shadowTransformMatrices.push_back(this->projectionMatrix * glm::lookAt(this->position, this->position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
            // right
            this->shadowTransformMatrices.push_back(this->projectionMatrix * glm::lookAt(this->position, this->position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
            // top
            this->shadowTransformMatrices.push_back(this->projectionMatrix * glm::lookAt(this->position, this->position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
            // bottom
            this->shadowTransformMatrices.push_back(this->projectionMatrix * glm::lookAt(this->position, this->position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
            // front
            this->shadowTransformMatrices.push_back(this->projectionMatrix * glm::lookAt(this->position, this->position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
            // back
            this->shadowTransformMatrices.push_back(this->projectionMatrix * glm::lookAt(this->position, this->position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        }
    }
};