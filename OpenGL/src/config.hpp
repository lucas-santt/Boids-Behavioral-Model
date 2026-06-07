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

const float triangleVertices[] = {
     // Coords           Colors
     0.0f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 1.0f,
     0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 1.0f
};