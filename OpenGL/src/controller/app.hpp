#pragma once

#include "../config.hpp"
#include "../model/scene.hpp"
#include "../view/renderer.hpp"

class App {
public:
    bool firstMouse = true;
    float lastX, lastY;

    App() {
        setUpGLFW();
        scene = new Scene();
        renderer = new Renderer(window);
    }

    ~App() {
        delete(scene);
        delete(renderer);
    }

    void run() {
        while(!glfwWindowShouldClose(window)) {
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            processInput();

            scene->update(deltaTime);

            renderer->render(scene);

            glfwPollEvents();
        }
    }

private:
    Scene* scene;
    Renderer* renderer;
    GLFWwindow* window;

    float deltaTime = 0.0f, lastFrame = 0.0f;

    void setUpGLFW() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Creating window
        window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Boids Simulation", NULL, NULL);
        if(window == NULL) {
            std::cout << "Failed to create GLFW window " << std::endl;
            glfwTerminate();
        }

        glfwSetWindowUserPointer(window, this);

        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    // TODO: Better architeture callbacks functions

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        App* app = static_cast<App*>(glfwGetWindowUserPointer(window));
        
        glViewport(0, 0, width, height);
        app->renderer->width  = width;
        app->renderer->height = height;
    }

    void processInput() {
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            scene->camera.ProcessKeyboard(FORWARD, deltaTime);
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            scene->camera.ProcessKeyboard(BACKWARD, deltaTime);
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            scene->camera.ProcessKeyboard(LEFT, deltaTime);
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            scene->camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    static void mouse_callback(GLFWwindow* window, double xPosIn, double yPosIn) {
        App* app = static_cast<App*>(glfwGetWindowUserPointer(window));
        float lastX = app->lastX;
        float lastY = app->lastY;
        
        float xPos = static_cast<float>(xPosIn);
        float yPos = static_cast<float>(yPosIn);

        if(app->firstMouse) {
            app->lastX = xPos;
            app->lastY = yPos;
            app->firstMouse = false;
        }

        float xOffset = xPos - lastX;
        float yOffset = lastY - yPos;
        app->lastX = xPos;
        app->lastY = yPos;

        app->scene->camera.ProcessMouseMovement(xOffset, yOffset);
    }

    static void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
        App* app = static_cast<App*>(glfwGetWindowUserPointer(window));
        app->scene->camera.ProcessMouseScroll(static_cast<float>(yOffset));
    }
};