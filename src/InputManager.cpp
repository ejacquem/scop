#include "InputManager.hpp"

InputManager::InputManager(GLFWwindow *window, Camera *camera, Scop *scop, LineDrawer *lineDrawer)
{
    this->camera = camera;
    this->scop = scop;
    this->lineDrawer = lineDrawer;

    lastX =  800.0f / 2.0;
    lastY =  600.0 / 2.0;
    mixValue = 0;
    // std::cout << "InputManager Constructor" << std::endl;

    glfwSetWindowUserPointer(window, this);
    this->set_callback_functions(window);
}

InputManager::~InputManager(){};

void InputManager::processInput(GLFWwindow *window)
{
    if (GET_KEY_PRESS(UP))
    {
        mixValue += 0.0001f;
        if(mixValue >= 1.0f)
            mixValue = 1.0f;
        // cout << "mixvalue: " << mixValue << endl;
    }
    if (GET_KEY_PRESS(DOWN))
    {
        mixValue -= 0.0001f;
        if (mixValue <= 0.0f)
            mixValue = 0.0f;
    }
    if (GET_KEY_PRESS(W))
        camera->processKeyboard(FORWARD, Time::deltaTime);
    if (GET_KEY_PRESS(S))
        camera->processKeyboard(BACKWARD, Time::deltaTime);
    if (GET_KEY_PRESS(A))
        camera->processKeyboard(LEFT, Time::deltaTime);
    if (GET_KEY_PRESS(D))
        camera->processKeyboard(RIGHT, Time::deltaTime);
    if (GET_KEY_PRESS(SPACE))
        camera->processKeyboard(UP, Time::deltaTime);
    if (GET_KEY_PRESS(LEFT_SHIFT))
        camera->processKeyboard(DOWN, Time::deltaTime);
}

// void processInput(GLFWwindow *window, float deltaTime){};
void InputManager::set_callback_functions(GLFWwindow *window)
{
    glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset)
    {
        InputManager *inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
        inputManager->scroll_callback(window, xoffset, yoffset);
    });
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos)
    {
        InputManager *inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
        inputManager->mouse_callback(window, xpos, ypos);
    });
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mode)
    {
        InputManager *inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
        inputManager->key_callback(window, key, scancode, action, mode);
    });
}

void InputManager::scroll_callback(GLFWwindow *window, double horizontal, double vertcial)
{
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera->processMouseScroll(vertcial, FOV_MODE);
    else
        camera->processMouseScroll(vertcial, SPEED_MODE);
    (void)horizontal;
}

void InputManager::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    this->lastX = xpos;
    this->lastY = ypos;

    camera->processMouseMovement(xoffset, yoffset);
    (void)window;
}

void InputManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
        scop->swap(1);
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
        scop->swap(-1);
    if (key == GLFW_KEY_F && action == GLFW_PRESS)
        lineDrawer->toggle();
    if (key == GLFW_KEY_C && action == GLFW_PRESS)
        camera->rotate = !camera->rotate;
    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
    {
        if(scop->color_palette_id < 5)
            scop->color_palette_id++;
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    {
        if(scop->color_palette_id > 0)
            scop->color_palette_id--;
    }
    if (key == GLFW_KEY_T && action == GLFW_PRESS)
        scop->color_mix_speed *= -1;
    if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        if(scop->rotation_speed == 0)
            scop->rotation_speed = Scop::DEFAULT_SPEED;
        else
            scop->rotation_speed = 0;
    }
    (void)scancode;
    (void)mode;
}
