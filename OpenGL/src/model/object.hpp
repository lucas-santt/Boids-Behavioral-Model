#pragma once

#include "../config.hpp"

class Object {
public:
    glm::vec3 position;
    glm::vec3 scale;
    bool wireframe;
    bool enabled;

    Object(
        glm::vec3 position,
        glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
        bool wireframe = false,
        bool enabled = true
    ) : position(position), scale(scale), wireframe(wireframe), enabled(enabled) { }

    void update(const float dt) { }

    glm::mat4 getModelTransform() {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        return glm::scale(model, scale);
    }
};