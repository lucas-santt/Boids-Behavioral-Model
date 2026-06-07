#pragma once

#include "../config.hpp"

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

class Camera 
{
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;

    float movementSpeed;
    float mouseSensitivity;
    float zoom; // Or FOV

    Camera(
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 up       = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw          = YAW,
        float pitch        = PITCH
    ) : front(glm::vec3(0.0f, 0.0f, -1.0f)),
        movementSpeed(SPEED),
        mouseSensitivity(SENSITIVITY), 
        zoom(ZOOM)
    {
        this->position = position;
        this->worldUp  = up;
        this->yaw      = yaw;
        this->pitch    = pitch;
        updateCameraVectors();
    }

    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
        : front(glm::vec3(0.0f, 1.0f, 0.0f)),
          movementSpeed(SPEED),
          mouseSensitivity(SENSITIVITY),
          zoom(ZOOM)
    {
        this->position = glm::vec3(posX, posY, posZ);
        this->worldUp  = glm::vec3(upX, upY, upZ);
        this->yaw      = yaw;
        this->pitch    = pitch;
        updateCameraVectors();
    }

    glm::mat4 GetView() {
        return glm::lookAt(position, position + front, up);
    }

    void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
        float velocity = movementSpeed * deltaTime;
        if(direction == FORWARD)
            position += front * velocity;
        else if(direction == BACKWARD)
            position -= front * velocity;
        else if(direction == LEFT)
            position -= right * velocity;
        else if(direction == RIGHT)
            position += right * velocity;
    }

    void ProcessMouseMovement(float xOffset, float yOffset, GLboolean constraintPitch = true) {
        xOffset *= mouseSensitivity;
        yOffset *= mouseSensitivity;

        yaw   += xOffset;
        pitch += yOffset;

        if(constraintPitch) {
            if(pitch > 89.0f)
                pitch = 89.0f;
            else if (pitch < -89.0f)
                pitch = -89.0f;
        }

        updateCameraVectors();
    }

    void ProcessMouseScroll(float yOffset) {
        zoom -= (float) yOffset;
        if(zoom < 1.0f)
            zoom = 1.0f;
        else if(zoom > 45.0f)
            zoom = 45.0f;
    }

private:
    void updateCameraVectors() {
        glm::vec3 newFront;
        newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        newFront.y = sin(glm::radians(pitch));
        newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        
        front = glm::normalize(newFront);
        right = glm::normalize(glm::cross(front, worldUp));
        up    = glm::normalize(glm::cross(right, front));
    }
};