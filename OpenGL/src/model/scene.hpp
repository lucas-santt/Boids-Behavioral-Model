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

    Scene() : camera(glm::vec3(-CUBE_AREA_LENGTH-5, CUBE_AREA_LENGTH+5, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, -45.0f) {
        srand(time(nullptr));
        // Ground
        cubes.push_back(Object(
            glm::vec3(0.0f, -(CUBE_AREA_LENGTH/2)-0.1, 0.0f), 
            glm::vec3(GROUND_LENGTH, 0.1f, GROUND_LENGTH)
        ));
        // Area wireframe
        cubes.push_back(Object(
            glm::vec3(0.0f),
            glm::vec3(CUBE_AREA_LENGTH),
            true
        ));

        for(int i=0; i<NUMBER_OF_BOIDS; i++) {
            boids.push_back(spawnRandomBoid());
        }
    }

    void update(const float dt) {
        for(Boid& boid : boids)
            boid.update(dt, boids);
    }

    void resetBoids() {
        for(int i=0; i<NUMBER_OF_BOIDS; i++)
            boids[i] = spawnRandomBoid();
    }

private:
    Boid spawnRandomBoid() {
        glm::vec3 pos     = randomVec3(-CUBE_AREA_LENGTH/2, CUBE_AREA_LENGTH/2);
        glm::vec3 forward = randomVec3(-1.0f, 1.0f);
        float init_vel    = randomFloat(BOID_MIN_SPEED, BOID_MAX_SPEED);
        return Boid(pos, BOID_SCALE, init_vel, forward);
    }

    glm::vec3 randomVec3(float low_bound, float high_bound) {
        return glm::vec3(
            randomFloat(low_bound, high_bound),
            randomFloat(low_bound, high_bound),
            randomFloat(low_bound, high_bound)
        );
    }

    float randomFloat(float low_bound, float high_bound) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(low_bound, high_bound);

        return dis(gen);
    }
};