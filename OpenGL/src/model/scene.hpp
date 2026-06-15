#pragma once

#include "../config.hpp"
#include "camera.hpp"
#include "object.hpp"
#include "boid.hpp"

class Scene {
public:
    Camera camera;
    std::vector<Boid> boids;
    std::vector<Object> cubes; 

    Scene() : camera(glm::vec3(0.0f, 10.0f, 0.0f)) {
        // Ground
        cubes.push_back(Object(
            glm::vec3(0.0f, 0.0f, 0.0f), 
            glm::vec3(10.0f, 0.1f, 10.0f)
        ));
        boids.push_back(Boid(glm::vec3(-5.0f, 2.0f, 0.0f)));
    }

    void update(const float dt) {
        for(Boid& boid : boids) {
            boid.update(dt);
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