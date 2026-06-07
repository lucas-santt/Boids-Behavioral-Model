#pragma once

#include "../config.hpp"
#include "camera.hpp"
#include "boid.hpp"

class Scene {
public:
    Camera camera;
    std::vector<Boid> boids;

    Scene() : camera(glm::vec3(0.0f, 0.0f, 0.0f)) {
        boids.push_back(Boid());
    }

    void update(float dt) {
        for(Boid& boid : boids) {
            boid.update();
        }
    }

    void move_player(glm::vec3 dPos) {
        std::cout << "move_player not implemented yet" << std::endl;
        return;
    }

    void spin_player(glm::vec3 dEulers) {
        std::cout << "spin_player not implemented yet" << std::endl;
        return;
    }
};