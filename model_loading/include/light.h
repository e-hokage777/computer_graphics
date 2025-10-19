#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader.h>
#include <glad/glad.h>
#include <cube.h>

namespace MyLight
{
    // constants
    glm::vec3 POSITION(0.0f, 0.0f, 0.0f);
    glm::vec3 COLOR(1.0f, 1.0f, 1.0f);
}

class Light
{
public:
    glm::vec3 position;
    glm::vec3 color;
    Shader shader;
    unsigned int vao, vbo;

    Light(Shader shader, glm::vec3 position = MyLight::POSITION, glm::vec3 color = MyLight::COLOR) : position(position), color(color), shader(shader)
    {
        glGenVertexArrays(1, &this->vao);
        glBindVertexArray(this->vao);

        glGenBuffers(1, &this->vbo);
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(CUBE_VERTICES), CUBE_VERTICES, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
    }

    void draw(glm::mat4 view, glm::mat4 projection)
    {
        this->shader.use();

        shader.setVec3("color", glm::value_ptr(this->color));

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.7));
        model = glm::translate(model, this->position);
        model = glm::rotate(model, glm::radians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        shader.setMat4("model", glm::value_ptr(model));
        shader.setMat4("view", glm::value_ptr(view));
        shader.setMat4("projection", glm::value_ptr(projection));

        glBindVertexArray(this->vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glUseProgram(0);
    }
};