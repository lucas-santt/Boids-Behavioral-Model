#pragma once

#include "../config.hpp"
#include "shader.hpp"

class Material 
{
public:
    Shader shader;

    Material(const char* vertexPath, const char* fragmentPath) 
        : shader(vertexPath, fragmentPath) {}

    ~Material() {
        glDeleteProgram(shader.ID);
    }
};