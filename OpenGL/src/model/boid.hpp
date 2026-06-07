#pragma once

#include "../config.hpp"

class Boid {
public:
    glm::vec3 position;

    Boid(
        glm::vec3 position = glm::vec3(0.0f, 0.0f, -3.0f)
    ) : position(position) {}

    void update() {
        return;
    }

    glm::mat4 getModelTransform() {
        glm::mat4 model = glm::mat4(1.0f);
        return glm::translate(model, position);
    }
};