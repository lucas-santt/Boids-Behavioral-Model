#pragma once

#include "../config.hpp"
#include "mesh.hpp"
#include "material.hpp"
#include "../model/scene.hpp"

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
        delete(boidMaterial);
        delete(cubeMesh);
        delete(cubeMaterial);
        delete(wireframeMaterial);
    }

    void render(Scene* scene, float time) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        
        std::vector<Shader> shaders = {
            boidMaterial->shader, 
            cubeMaterial->shader,
            wireframeMaterial->shader
        };
        
        glm::mat4 projection = glm::perspective(
            glm::radians(scene->camera.zoom), 
            (float)this->width / (float)this->height, 
            0.1f, 
            100.0f
        );

        for(Shader& s: shaders) {
            s.use();
            s.setMat4("projection", projection);
            s.setMat4("view", scene->camera.GetView());
        }

        // Render Scene Obejcts 

        boidMaterial->shader.use();
        for(Boid& boid : scene->boids) {
            boidMaterial->shader.setFloat("uTime", time);
            boidMaterial->shader.setMat4("model", boid.getModelTransform());
            triangleMesh->draw();
        }

        // Fill cubes
        cubeMaterial->shader.use();
        for(Object& cube : scene->cubes) {
            if(cube.wireframe || !cube.enabled) continue;
            cubeMaterial->shader.setMat4("model", cube.getModelTransform());
            cubeMesh->draw();
        }

        // Wireframes cubes
        wireframeMaterial->shader.use();
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        for(Object&cube: scene->cubes) {
            if(!cube.wireframe || !cube.enabled) continue;
            wireframeMaterial->shader.setMat4("model", cube.getModelTransform());
            cubeMesh->draw();
        }

        glfwSwapBuffers(window);
    }

private:
    GLFWwindow* window;
    Mesh* triangleMesh;
    Mesh* cubeMesh;
    Material* boidMaterial;
    Material* cubeMaterial;
    Material* wireframeMaterial;

    void setUpOpenGL(GLFWwindow* window) {
        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
        }
        
        glEnable(GL_DEPTH_TEST);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    void makeAssets() {
        triangleMesh = new Mesh(TRIANGLE_VERTICES);
        boidMaterial = new Material(
            "src/shaders/boid_shader.vs", 
            "src/shaders/default_shader.fs"
        );

        cubeMesh = new Mesh(CUBE_VERTICES, CUBE_INDICES);
        cubeMaterial = new Material(
            "src/shaders/default_shader.vs",
            "src/shaders/default_shader.fs"
        );

        wireframeMaterial = new Material(
            "src/shaders/default_shader.vs",
            "src/shaders/wireframe_shader.fs"
        );
    }
};