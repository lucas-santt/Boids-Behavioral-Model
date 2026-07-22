#pragma once

#include "../config.hpp"
#include "spatialHash.hpp"

class Boid {
public:
    int id;
    glm::ivec3 cellIndex;

    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 steering; // "Acceleration" vector
    float speed;

    Boid(
        int id,
        glm::vec3 position       = glm::vec3(0.0f),
        glm::vec3 scale          = glm::vec3(1.0f),
        float     init_velocity  = 0.0f,
        glm::vec3 forward        = glm::vec3(0.0f, 0.0, -1.0f),
        glm::vec3 worldUp        = glm::vec3(0.0f, 1.0f, 0.0f)
    ) : id(id),
        position(position),
        scale(scale),
        steering(forward), 
        speed(init_velocity),
        forward(forward),
        worldUp(worldUp)
    { }

    void update(const float dt, std::span<Boid> boids, SpatialHashGrid& spatialGrid) {
        boidBehavior(boids, spatialGrid);
        geometricFlight(dt);
        checkBoundaries();

        spatialGrid.update(this->id, this->position, BOID_VISION_RADIUS, this->cellIndex);
    }

    glm::mat4 getModelTransform() {
        /* Check SongHo article about OpenGL Transformation if any doubts */
        glm::vec3 gravity = -worldUp;

        // localUp according to banking
        glm::vec3 bankUp = steering - gravity;

        glm::vec3 right  = glm::normalize(glm::cross(forward, bankUp));
        // bankUp isn't always perpendicular to forward, so we create a localUp 
        glm::vec3 localUp = glm::normalize(glm::cross(forward, right));

        glm::mat4 model = glm::mat4(1.0f);
        
        // Matrix indices represent columns
        model[0] = glm::vec4(right,    0.0f);
        model[1] = glm::vec4(localUp,  0.0f);
        model[2] = glm::vec4(forward,  0.0f);
        model[3] = glm::vec4(position, 1.0f);

        model = glm::scale(model, scale);

        return model;
    }

private:
    glm::vec3 forward;
    glm::vec3 worldUp;

    void geometricFlight(float dt) {
        glm::vec3 velocity = (forward * speed) + (steering * dt);

        float curr_speed = glm::length(velocity); // Can use length2 instead
        if(curr_speed > BOID_MAX_SPEED) {
            velocity = glm::normalize(velocity) * BOID_MAX_SPEED;
            curr_speed = BOID_MAX_SPEED;
        } else if(curr_speed < BOID_MIN_SPEED) {
            velocity = glm::normalize(velocity) * BOID_MIN_SPEED;
            curr_speed = BOID_MIN_SPEED;
        }

        position += velocity * dt;
        if(curr_speed != 0) forward = glm::normalize(velocity);
    }

    void checkBoundaries() {
        float half_length = CUBE_AREA_LENGTH/2;
        if(position.x < -half_length || position.x > half_length){
            position.x = (position.x < 0) ? -half_length : half_length;
            steering.x = -steering.x;
            forward.x = -forward.x;
            forward = glm::normalize(forward);
        }
        if(position.y < -half_length || position.y > half_length) {
            position.y = (position.y < 0) ? -half_length : half_length;
            steering.y = -steering.y;
            forward.y = -forward.y;
            forward = glm::normalize(forward);
        }
        if(position.z < -half_length || position.z > half_length) {
            position.z = (position.z < 0) ? -half_length : half_length;
            steering.z = -steering.z;
            forward.z = -forward.z;
            forward = glm::normalize(forward);
        }
    }

    void boidBehavior(std::span<Boid> boids, SpatialHashGrid& spatialGrid) {
        glm::vec3 collision_avoidance = glm::vec3(0.0f);
        glm::vec3 velocity_matching   = this->forward * this->speed;
        glm::vec3 flock_centering     = this->position;
        int nearby_boids = 1; // Consider itself

        std::vector<int> neighbors = spatialGrid.getNeighbors(position, BOID_VISION_RADIUS);

        for(int nID: neighbors) {
            if(nID == this->id) continue;
            Boid b = boids[nID];

            float distance = glm::length(b.position - this->position);
            if(distance > BOID_VISION_RADIUS) continue;
            
            // Collision Avoidance
            glm::vec3 repulse = this->position - b.position;
            if(glm::length(repulse) != 0) collision_avoidance += glm::normalize(repulse) / distance;
            // Division by 0 if both are on the same position

            // Velocity Matching
            velocity_matching += b.forward * b.speed;
            
            // Flock Centering
            flock_centering += b.position;

            nearby_boids++;
        }

        if(nearby_boids == 1) return;

        collision_avoidance = glm::normalize(collision_avoidance);
        
        velocity_matching /= (float)nearby_boids;
        velocity_matching = glm::normalize(velocity_matching);
        
        flock_centering /= (float)nearby_boids;
        flock_centering = flock_centering - this->position; 
        flock_centering = glm::normalize(flock_centering);


        glm::vec3 dir = PIAA(collision_avoidance, velocity_matching, flock_centering);
        steering = dir;
        speed = glm::length(dir);

        if(glm::length(steering) > 0.001f) steering = glm::normalize(steering);
    }

    glm::vec3 PIAA(glm::vec3 ca, glm::vec3 vm, glm::vec3 fc) {
        /* Prioritized acceleration allocation */
        glm::vec3 accumulator = glm::vec3(0.0f);
        float acc_mag = 0.0f;

        glm::vec3 requests[3] = {
            ca * BOID_COLLISION_AVOIDANCE_WEIGHT, 
            vm * BOID_VELOCITY_MATCHING_WEIGHT, 
            fc * BOID_FLOCK_CENTERING_WEIGHT
        };

        for(int i=0; i<3; i++) {
            float magnitude = glm::length(requests[i]);
            if(magnitude < 0.01f) continue;

            if(acc_mag + magnitude <= BOID_MAX_ACCELERATION) {
                accumulator += requests[i];
                acc_mag += magnitude;
            } else {
                float remaining = BOID_MAX_ACCELERATION - acc_mag; // Remaining magnitude

                if(remaining > 0.0f) {
                    // Add the remaing vector
                    glm::vec3 remaining_request = (requests[i] / magnitude) * remaining;
                    accumulator += remaining_request;
                }

                break; // Runned out of budget
            }
        }
        return accumulator;
    }
};