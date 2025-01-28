#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.hpp"
#include "Scop.hpp"
#include "LineDrawer.hpp"

#define GET_KEY(key) glfwGetKey(window, GLFW_KEY_##key)
#define GET_KEY_PRESS(key) glfwGetKey(window, GLFW_KEY_##key) == GLFW_PRESS
#define GET_KEY_RELEASE(key) glfwGetKey(window, GLFW_KEY_##key) == GLFW_RELEASE

class InputManager
{
    enum {
        MOVE_CAMERA,
        MOVE_OBJECT
    };

private:
    Camera *camera;
    Scop *scop;
    LineDrawer *lineDrawer;

    float lastX;
    float lastY;
    float mixValue;
    int inputMode;
    bool wireframe = false;
    bool capturing_mouse = false;
    bool firstMouse = true;

public:
    InputManager(GLFWwindow *window, Camera *camera, Scop *scop, LineDrawer *lineDrawer);
    ~InputManager();

    // process key that are pressed continuously
    void processInput(GLFWwindow *window);

    void set_callback_functions(GLFWwindow *window);

    // call_back functions for key that are pressed once

    void scroll_callback(GLFWwindow *window, double horizontal, double vertcial);
    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
    void window_focus_callback(GLFWwindow* window, int focused);
    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
};

#endif