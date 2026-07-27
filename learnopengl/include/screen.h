#pragma once

#include "shader.h"
#include "mesh.h"
#include <vector>

class Screen
{
public:
    unsigned int width;
    unsigned int height;
    unsigned int samples;

    Screen(unsigned int width = 800, unsigned int height = 600, unsigned int samples = 1) : width(width), height(height), samples(samples)
    {
        this->setup();
    }

    void draw(Shader shader)
    {
        shader.use();
        shader.setTexUnit(0, this->renderTexture, "sampler", GL_TEXTURE_2D);
        glBindVertexArray(this->vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void activate()
    {
        // glBindFramebuffer(GL_READ_FRAMEBUFFER, this->fbo);
        // glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->renderFBO);
        // glBlitFramebuffer(0, 0, this->width, this->height, 0, 0, this->width, this->height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

        // glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
        // glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

        // glBindFramebuffer(GL_FRAMEBUFFER, this->renderFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
    }

    void deactivate()
    {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, this->fbo);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->renderFBO);
        glBlitFramebuffer(0, 0, this->width, this->height, 0, 0, this->width, this->height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

private:
    unsigned int vbo;
    unsigned int vao;
    unsigned int ebo;
    unsigned int fbo;
    unsigned int renderFBO;
    unsigned int texture;
    unsigned int renderTexture;
    unsigned int depthStencilBuffer;
    unsigned int renderDepthStencilBuffer;

    void setup()
    {

        // defining vertex data
        std::vector<Vertex> vertices = {
            Vertex{glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)},
            Vertex{glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)},
            Vertex{glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)},
            Vertex{glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)}};

        // defining indices
        std::vector<unsigned int> indices = {
            0, 1, 2,
            2, 3, 0};

        // creating buffer
        glGenBuffers(1, &this->vbo);
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenVertexArrays(1, &this->vao);

        glBindVertexArray(this->vao);
        // defning vertex attributes
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));

        // glBindBuffer(GL_ARRAY_BUFFER, 0);
        glGenBuffers(1, &this->ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        glBindVertexArray(0);

        this->configureFBO();
    }

    void configureFBO()
    {
        bool multisampled = this->samples > 1;

        // create textures
        glGenTextures(1, &this->texture);
        glBindTexture(multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, this->texture);
        // glTexParameteri(multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        // glTexParameteri(multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR)
        {
            std::cout << "GL Error: " << err << std::endl;
        }

        glGenRenderbuffers(1, &this->depthStencilBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, this->depthStencilBuffer);

        if (multisampled)
        {

            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, this->texture);
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, this->samples, GL_RGB, this->width, this->height, GL_TRUE);

            // glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, this->depthStencilBuffer);
            // glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, this->samples, GL_DEPTH24_STENCIL8, this->width, this->height, GL_TRUE);
            // glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, this->samples, GL_DEPTH24_STENCIL8, this->width, this->height);
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D, this->texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

            // glBindTexture(GL_TEXTURE_2D, this->depthStencilBuffer);
            // glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, this->width, this->height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->width, this->height);
        }
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        //// create and configure fbo
        glGenFramebuffers(1, &this->fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, this->texture, 0);
        // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, this->depthStencilBuffer, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->depthStencilBuffer);

        // check if framebuffer is complete
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        else
            std::cout << "SUCCESS: Framebuffer setup" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, 0);

        // creating render buffer
        glGenFramebuffers(1, &this->renderFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, this->renderFBO);

        // creating render buffer texture
        glGenTextures(1, &this->renderTexture);
        glBindTexture(GL_TEXTURE_2D, this->renderTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->renderTexture, 0);

        // creating depth buffer
        glGenRenderbuffers(1, &this->renderDepthStencilBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, this->renderDepthStencilBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->width, this->height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->renderDepthStencilBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        // check if framebuffer is complete
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Render Framebuffer is not complete!" << std::endl;
        else
            std::cout << "SUCCESS: Render Framebuffer setup" << std::endl;

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};