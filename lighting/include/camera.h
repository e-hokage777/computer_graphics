#pragma once

#include <iostream>
#include <glad/gl.h>
#include <glm/gtc/matrix_transform.hpp>

// constants
const glm::vec3 CAMERA_WOLRD_UP = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 CAMERA_FRONT = glm::vec3(0.0f, 0.0f, 1.0f);
const glm::vec3 CAMERA_POSITION = glm::vec3(0.0f, 0.0f, 0.0f);
const float SENSITIVITY = 0.1f;
const float SPEED = 0.00005f;

enum CameraDirection
{
    Forward,
    Backward,
    Left,
    Right
};

class Camera
{
public:
    glm::vec3 position;
    glm::vec3 front;
    float sensitivity;
    float speed;
    Camera(glm::vec3 position = CAMERA_POSITION,
           glm::vec3 target = CAMERA_FRONT,
           float sensitivity = SENSITIVITY,
           float speed = SPEED) : position(position), sensitivity(sensitivity), speed(speed), front(CAMERA_FRONT)
    {
    }

    glm::mat4 getMatrix()
    {
        // would have to compute through values
        return glm::lookAt(position, front, CAMERA_WOLRD_UP);
    }

    // function to move camera
    void move(CameraDirection direction)
    {
        glm::vec3 right;
        switch (direction)
        {
        case CameraDirection::Forward:
            this->position += (glm::normalize(this->front) * speed);
            break;
        case CameraDirection::Backward:
            this->position -= (glm::normalize(this->front) * speed);
            break;
        case CameraDirection::Left:
            right = glm::normalize(glm::cross(CAMERA_WOLRD_UP, this->front));
            this->position -= right * speed;
            break;
        case CameraDirection::Right:
            right = glm::normalize(glm::cross(CAMERA_WOLRD_UP, this->front));
            this->position += right * speed;
            break;
        }
    }

private:
};