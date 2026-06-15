#pragma once

#include <iostream>
#include <vector>
#include <span>

#include <fstream>
#include <sstream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtc/constants.hpp>

// Triangle is based on z-axis because
// boids need to point to it's forward direction (z-axis)
const float TRIANGLE_VERTICES[] = {
     // Coords           Colors
      0.0f, 0.0f,  0.5f,  0.6f, 0.6f, 1.0f,
      0.5f, 0.0f, -0.5f,  0.6f, 0.6f, 1.0f,
     -0.5f, 0.0f, -0.5f,  0.6f, 0.6f, 1.0f
};

const float CUBE_VERTICES[] = {
     // Coords             Colors
      0.5f,  0.5f,  0.5f,  0.0f, 0.7f, 0.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
     -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
     -0.5f,  0.5f,  0.5f,  0.0f, 0.5f, 0.0f,
      0.5f,  0.5f, -0.5f,  0.0f, 0.9f, 0.0f,
      0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
     -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
     -0.5f,  0.5f, -0.5f,  0.0f, 0.3f, 0.0f
};

const unsigned int CUBE_INDICES[] = {
    0, 3, 2,  2, 1, 0,
    4, 5, 6,  6, 7, 4,
    0, 1, 5,  5, 4, 0,
    3, 7, 6,  6, 2, 3,
    4, 7, 3,  3, 0, 4,
    1, 2, 6,  6, 5, 1
};

const float BOID_MIN_SPEED = 0.0f;
const float BOID_MAX_SPEED = 10.0f;

const float CUBE_AREA_LENGTH = 10.0f;