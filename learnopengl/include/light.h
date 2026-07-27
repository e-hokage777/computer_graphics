#pragma once

#include <iostream>
#include "glm/gtc/matrix_transform.hpp"
#include "cube.h"
#include "shader.h"

// constants
const glm::vec3 LIGHT_POSITION = glm::vec3(0.0f, 0.0f, 0.0f);
const glm::vec3 LIGHT_COLOR = glm::vec3(1.0f, 1.0f, 1.0f);
const float LIGHT_INTENSITY = 1.0f;
const float LIGHT_ANGLE = 0.0f;
const float LIGHT_FALLOFF = 0.0f;

class Light
{
public:
    glm::vec3 position;
    glm::vec3 color;
    float intensity;
    float angle;
    float falloff;
    Cube object;

    Light(glm::vec3 position = LIGHT_POSITION, glm::vec3 color = LIGHT_COLOR, float intensity = LIGHT_INTENSITY, float angle = LIGHT_ANGLE, float falloff = LIGHT_FALLOFF)
    {
        this->position = position;
        this->color = color;
        this->intensity = intensity;
        this->angle = angle;
        this->falloff = falloff;

        object = Cube(glm::vec3(1.0f), this->color, this->position);
    }

    void Draw(Shader shader)
    {
        object.Draw(shader);
    }
};
