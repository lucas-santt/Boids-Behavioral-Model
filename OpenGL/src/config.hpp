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
const float triangleVertices[] = {
     // Coords           Colors
      0.0f, 0.0f,  0.5f,  1.0f, 1.0f, 1.0f,
      0.5f, 0.0f, -0.5f,  1.0f, 1.0f, 1.0f,
     -0.5f, 0.0f, -0.5f,  1.0f, 1.0f, 1.0f
};

const float BOID_MIN_SPEED = 0.0f;
const float BOID_MAX_SPEED = 10.0f;