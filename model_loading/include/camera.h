#pragma once

#include <glm/gtc/matrix_transform.hpp>

// constants
glm::vec3 POSITION = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 FRONT = glm::vec3(0.0f, 0.0f, -1.0f);
float SENSITIVITY = 2.0f;
float SPEED = 5.0f;
float YAW = -90.0f;
float PITCH = 0;

enum CameraMovement
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
    glm::vec3 up;
    float speed;
    float sensitivity;
    float yaw;
    float pitch;
    Camera(glm::vec3 position = POSITION, glm::vec3 front = FRONT, glm::vec3 up = UP, float speed = SPEED, float sensitivity = SENSITIVITY)
        : position(position), front(front), up(up), speed(speed), sensitivity(sensitivity)
    {
        yaw = YAW;
        pitch = PITCH;
    }

    void move(CameraMovement direction, float deltaTime)
    {
        glm::vec3 left = glm::normalize(glm::cross(this->up, this->front));

        float velocity = (this->speed * deltaTime);
        switch (direction)
        {
        case CameraMovement::Forward:
            this->position = this->position + this->front * velocity;
            break;
        case CameraMovement::Backward:
            this->position = this->position - this->front * velocity;
            break;
        case CameraMovement::Left:
            this->position = this->position + left * velocity;
            break;
        case CameraMovement::Right:
            this->position = this->position - left * velocity;
            break;
        }
    }

    void rotate(float dx, float dy, float deltaTime)
    {
        float rotationSpeed = deltaTime * sensitivity;

        // std::cout << "dx: " << dx << " deltaTime: " << deltaTime << std::endl;

        yaw += dx * rotationSpeed;
        pitch += dy * rotationSpeed;

        if (pitch >= 89.0f)
        {
            pitch = 89.0f;
        }
        else if (pitch <= -89.0f)
        {
            pitch = -89.0f;
        }

        // this->front.x = glm::cos(glm::radians(yaw));
        // // this->front.y = glm::sin(glm::radians(yaw));
        // this->front.z = glm::sin(glm::radians(yaw));
        this->front.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
        this->front.y = glm::sin(glm::radians(pitch));
        this->front.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    }

    glm::mat4 getMatrix()
    {
        this->front = glm::normalize(this->front);
        return glm::lookAt(this->position, this->position + this->front, this->up);
    }
};