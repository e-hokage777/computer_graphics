#include <iostream>
#include "glm/gtc/matrix_transform.hpp"
#include "glad/gl.h"
#include "shader.h"
#include "scene.h"
#include <vector>

const int SHADOW_WIDTH = 1024;
const int SHADOW_HEIGHT = 1024;

struct ShadowMapVertex
{
    glm::vec3 pos;
    glm::vec2 texCoords;
};

class DirectionalLight
{
public:
    glm::vec3 direction;
    glm::vec3 color;
    glm::vec3 falsePosition;
    unsigned int shadowMap;
    glm::mat4 lightViewMat;
    glm::mat4 lightMatrix;
    glm::mat4 projectionMat;

    DirectionalLight(glm::vec3 direction, glm::vec3 color, glm::vec3 falsePosition)
    {
        this->direction = direction;
        this->color = color;
        this->falsePosition = falsePosition;

        this->setup();
    }

    void renderShadowMap(Shader shader, Scene scene)
    {
        shader.use();
        shader.uniformMat4("view", this->lightViewMat);
        shader.uniformMat4("projection", this->projectionMat);
        // shader.uniformVec3("lightColor", this->color);

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
        glClear(GL_DEPTH_BUFFER_BIT);
        glCullFace(GL_FRONT);
        scene.render(shader);
        glCullFace(GL_BACK);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void drawShadowMap(Shader shader)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->shadowMap);

        glDisable(GL_DEPTH_TEST);

        shader.use();
        glBindVertexArray(this->vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glEnable(GL_DEPTH_TEST);
    }

    // void resizeDepthMap(int width, int height)
    // {

    // }

private:
    unsigned int framebuffer;
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
    std::vector<ShadowMapVertex> vertices;

    void setup()
    {
        // init texture
        glGenTextures(1, &this->shadowMap);
        glBindTexture(GL_TEXTURE_2D, this->shadowMap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL); // TODO: watch here well
        glBindTexture(GL_TEXTURE_2D, 0);

        glGenFramebuffers(1, &this->framebuffer);

        glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->shadowMap, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

        // setting up light view mat
        this->lightViewMat = glm::lookAt(this->falsePosition, glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        // setting up orthographic projection matrix
        this->projectionMat = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 15.0f);
        // this->projectionMat = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 200.0f);

        this->lightMatrix = this->projectionMat * this->lightViewMat;

        // setting up shader
        // this->shader = Shader("shaders/single_tex_shader.vs", "shaders/single_tex_shader.fs");

        vertices.push_back(ShadowMapVertex{glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)});  // top left
        vertices.push_back(ShadowMapVertex{glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)}); // bottom left
        vertices.push_back(ShadowMapVertex{glm::vec3(1.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)});  // bottom right
        vertices.push_back(ShadowMapVertex{glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)});   // top right

        std::vector<unsigned int> indices = {
            0, 1, 2,
            0, 2, 3};

        // setting up vao and vbo
        glGenVertexArrays(1, &this->vao);
        glGenBuffers(1, &this->vbo);
        glGenBuffers(1, &this->ebo);

        glBindVertexArray(this->vao);
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(ShadowMapVertex), &vertices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ShadowMapVertex), (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ShadowMapVertex), (void *)offsetof(ShadowMapVertex, texCoords));
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenBuffers(1, &this->ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        glBindVertexArray(0);
    }
};