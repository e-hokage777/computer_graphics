#pragma once

#include <iostream>
#include <glad/gl.h>
#include <glm/gtc/matrix_transform.hpp>

// constants
const glm::vec3 CAMERA_WOLRD_UP = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 CAMERA_DIRECTION = glm::vec3(0.0f, 0.0f, 1.0f);
const glm::vec3 CAMERA_POSITION = glm::vec3(0.0f, 0.0f, 0.0f);
const float SENSITIVITY = 0.1f;

class Camera
{
public:
    glm::vec3 position;
    glm::vec3 direction;
    float sensitivity;
    Camera(glm::vec3 position = CAMERA_POSITION, glm::vec3 direction = CAMERA_DIRECTION, float sensitivity = SENSITIVITY) : position(position), direction(direction), sensitivity(sensitivity)
    {
    }

    glm::mat4 getMatrix()
    {
        // would have to compute through values
        return glm::lookAt(position, direction, CAMERA_WOLRD_UP);
    }

private:
};