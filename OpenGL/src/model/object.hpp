#pragma once

#include "../config.hpp"

class Object {
public:
    glm::vec3 position;
    glm::vec3 scale;

    Object(
        glm::vec3 position,
        glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)
    ) : position(position), scale(scale) { }

    void update(const float dt) { }

    glm::mat4 getModelTransform() {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        return glm::scale(model, scale);
    }
};