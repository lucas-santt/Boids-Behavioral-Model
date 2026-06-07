#pragma once

#include "../config.hpp"

class Mesh
{
public:
    unsigned int vertex_size;

    Mesh(std::span<const float> vertices) {
        vertex_size = vertices.size() / 6;
        generateVAO(vertices);
    }

    ~Mesh() {
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
    }

    void draw() {
        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, vertex_size);

        glBindVertexArray(0);
    }

private:
    unsigned int VAO, VBO;

    void generateVAO(std::span<const float> vertices) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, vertices.size_bytes(), vertices.data(), GL_STATIC_DRAW);
        
        // Position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        // Color
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
};