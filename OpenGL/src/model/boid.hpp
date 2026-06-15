#pragma once

#include "../config.hpp"

class Boid {
public:
    glm::vec3 position;
    glm::vec3 steering; // "Acceleration" vector
    float speed;

    Boid(
        glm::vec3 position       = glm::vec3(0.0f, -1.0f, -3.0f),
        float     init_velocity  = 2.0f,
        glm::vec3 forward        = glm::vec3(0.0f, 0.0, -1.0f),
        glm::vec3 worldUp        = glm::vec3(0.0f, 1.0f, 0.0f)
    ) : position(position),
        steering(glm::vec3(1.0f, 0.0f, 0.0f)), // Temporary (Needs to be forward) 
        speed(init_velocity),
        forward(forward),
        worldUp(worldUp)
    { }

    void update(const float dt) {
        glm::vec3 right = glm::normalize(glm::cross(forward, worldUp));
        glm::vec3 up    = glm::normalize(glm::cross(right, forward));
        worldSteering = (right * steering.x) + (up * steering.y) + (forward * steering.z);

        glm::vec3 velocity = (forward * speed) + (worldSteering * dt);

        float curr_speed = glm::length(velocity); // Can use length2 instead
        if(curr_speed > BOID_MAX_SPEED)
            velocity = glm::normalize(velocity) * BOID_MAX_SPEED;
        else if(curr_speed < BOID_MIN_SPEED)
            velocity = glm::normalize(velocity) * BOID_MIN_SPEED;

        position += velocity * dt;
        forward = glm::normalize(velocity);
    }

    glm::mat4 getModelTransform() {
        /* Check SongHo article about OpenGL Transformation if any doubts */
        glm::vec3 gravity = -worldUp;

        // localUp according to banking
        glm::vec3 bankUp = worldSteering - gravity;

        glm::vec3 right  = glm::normalize(glm::cross(forward, bankUp));
        // bankUp isn't always perpendicular to forward, so we create a localUp 
        glm::vec3 localUp = glm::normalize(glm::cross(forward, right));

        glm::mat4 model = glm::mat4(1.0f);
        
        // Matrix indices represent columns
        model[0] = glm::vec4(right,    0.0f);
        model[1] = glm::vec4(localUp,  0.0f);
        model[2] = glm::vec4(forward,  0.0f);
        model[3] = glm::vec4(position, 1.0f);

        return model;
    }

private:
    glm::vec3 forward;
    glm::vec3 worldUp;
    glm::vec3 worldSteering; // Will we need it in the future???
};