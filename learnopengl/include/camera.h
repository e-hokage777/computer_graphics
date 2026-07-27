#pragma once

#include <glm/gtc/matrix_transform.hpp>

glm::vec3 WORLD_UP = glm::vec3(0, 1, 0);
const float SPEED = 0.1f;

enum CameraMovement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera
{
public:
    glm::vec3 position;
    glm::vec3 forward;
    glm::vec3 up;
    float pitch;
    float yaw;
    float speed;
    float sensitivity;
    Camera(glm::vec3 position, glm::vec3 direction, float speed = 0.01, float sensitivity = 0.001) : position(position), forward(direction), up(WORLD_UP), speed(SPEED), pitch(0), yaw(-90)
    {
        this->sensitivity = sensitivity;
    }

    void move(CameraMovement movement)
    {
        if (movement == CameraMovement::FORWARD)
        {
            position += forward * 0.1f;
        }
        else if (movement == CameraMovement::BACKWARD)
        {
            position -= forward * 0.1f;
        }
        else if (movement == CameraMovement::LEFT)
        {
            position -= glm::normalize(glm::cross(this->forward, this->up)) * 0.1f;
        }
        else if (movement == CameraMovement::RIGHT)
        {
            position += glm::normalize(glm::cross(this->forward, this->up)) * 0.1f;
        }
    }

    void updateDirection(float dx, float dy)
    {
        this->pitch += dy * this->sensitivity;
        this->yaw += dx * this->sensitivity;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        this->forward.x = cos(this->yaw) * cos(this->pitch);
        this->forward.y = sin(this->pitch);
        this->forward.z = sin(this->yaw) * cos(this->pitch);
        this->forward = glm::normalize(this->forward);
    }

    glm::mat4 getMatrix()
    {

        // compute right vector
        glm::mat4 viewMatrix = glm::lookAt(position, position + this->forward, this->up);
        return viewMatrix;
    }
};