#pragma once

#include <iostream>
#include <glad/gl.h>
#include <glm/gtc/matrix_transform.hpp>

// constants
const glm::vec3 CAMERA_WOLRD_UP = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 CAMERA_FRONT = glm::vec3(0.0f, 0.0f, 1.0f);
const glm::vec3 CAMERA_POSITION = glm::vec3(0.0f, 0.0f, 0.0f);
const float SENSITIVITY = 20.0f;
const float SPEED = 4.0f;

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
    float deltaTime = 0;
    glm::vec3 position;
    glm::vec3 front;
    float sensitivity;
    float speed;
    float yaw = 90.0f;
    Camera(glm::vec3 position = CAMERA_POSITION,
           glm::vec3 target = CAMERA_FRONT,
           float sensitivity = SENSITIVITY,
           float speed = SPEED) : position(position), sensitivity(sensitivity), speed(speed), front(CAMERA_FRONT)
    {
    }

    glm::mat4 getMatrix()
    {
        // computing front
        front.x = glm::cos(glm::radians(yaw));
        front.z = glm::sin(glm::radians(yaw));


        front = glm::normalize(front);

        // would have to compute through values
        return glm::lookAt(position, position + front, CAMERA_WOLRD_UP);
    }

    // function to move camera
    void move(CameraDirection direction)
    {
        glm::vec3 right;
        switch (direction)
        {
        case CameraDirection::Forward:
            this->position += (glm::normalize(this->front) * speed * deltaTime);
            break;
        case CameraDirection::Backward:
            this->position -= (glm::normalize(this->front) * speed * deltaTime);
            break;
        case CameraDirection::Left:
            right = glm::normalize(glm::cross(CAMERA_WOLRD_UP, this->front));
            this->position -= right * speed * deltaTime;
            break;
        case CameraDirection::Right:
            right = glm::normalize(glm::cross(CAMERA_WOLRD_UP, this->front));
            this->position += right * speed * deltaTime;
            break;
        }
    }

    void computeDirection(float dx, float dy){
        yaw += dx * deltaTime * sensitivity;
    }

    void update(float deltaTime){
        this->deltaTime = deltaTime;
    }

private:
};