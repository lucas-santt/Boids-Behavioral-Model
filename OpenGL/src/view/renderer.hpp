#pragma once

#include "../config.hpp"
#include "mesh.hpp"
#include "material.hpp"
#include "../model/scene.hpp"

const int SCR_WIDTH = 600;
const int SCR_HEIGHT = 800;

class Renderer {
public:
    int width, height;

    Renderer(GLFWwindow* window) : width(SCR_WIDTH), height(SCR_HEIGHT) {
        this->window = window;
        setUpOpenGL(window);
        makeAssets();
    }

    ~Renderer() {
        delete(triangleMesh);
        delete(triangleMaterial);
    }

    void render(Scene* scene) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        Shader triangleShader = triangleMaterial->shader;
        triangleShader.use();
        
        glm::mat4 projection = glm::perspective(
            glm::radians(scene->camera.zoom), 
            (float)this->width / (float)this->height, 
            0.1f, 
            100.0f
        );
        triangleShader.setMat4("projection", projection);
        triangleShader.setMat4("view", scene->camera.GetView());

        for(Boid& boid : scene->boids) {
            triangleShader.setMat4("model", boid.getModelTransform());
            triangleMesh->draw();
        }

        glfwSwapBuffers(window);
    }

private:
    GLFWwindow* window;
    Mesh* triangleMesh;
    Material* triangleMaterial;

    void setUpOpenGL(GLFWwindow* window) {
        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
        }
        
        glEnable(GL_DEPTH_TEST);
    }

    void makeAssets() {
        triangleMesh = new Mesh(triangleVertices);
        triangleMaterial = new Material(
            "src/shaders/default_shader.vs", 
            "src/shaders/default_shader.fs"
        );
    }
};