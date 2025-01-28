#include "InputManager.hpp"

InputManager::InputManager(GLFWwindow *window, Camera *camera, Scop *scop, LineDrawer *lineDrawer)
{
    this->camera = camera;
    this->scop = scop;
    this->lineDrawer = lineDrawer;

    lastX =  800.0f / 2.0;
    lastY =  600.0 / 2.0;
    mixValue = 0;
    inputMode = InputManager::MOVE_CAMERA;
    // std::cout << "InputManager Constructor" << std::endl;

    glfwSetWindowUserPointer(window, this);
    this->set_callback_functions(window);
}

InputManager::~InputManager(){};

void InputManager::processInput(GLFWwindow *window)
{
    if (inputMode == InputManager::MOVE_CAMERA){
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
    if (inputMode == InputManager::MOVE_OBJECT){
        if (GET_KEY_PRESS(W))
            scop->position.x += scop->move_speed * Time::deltaTime;
        if (GET_KEY_PRESS(S))
            scop->position.x -= scop->move_speed * Time::deltaTime;
        if (GET_KEY_PRESS(A))
            scop->position.z -= scop->move_speed * Time::deltaTime;
        if (GET_KEY_PRESS(D))
            scop->position.z += scop->move_speed * Time::deltaTime;
        if (GET_KEY_PRESS(SPACE))
            scop->position.y += scop->move_speed * Time::deltaTime;
        if (GET_KEY_PRESS(LEFT_SHIFT))
            scop->position.y -= scop->move_speed * Time::deltaTime;
    }
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
    glfwSetWindowFocusCallback(window, [](GLFWwindow* window, int focused)
    {
        InputManager *inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
        inputManager->window_focus_callback(window, focused);
    });
    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
    {
        InputManager *inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
        inputManager->mouse_button_callback(window, button, action, mods);
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
    if (capturing_mouse == false)
        return;

    if (firstMouse) {
        this->lastX = xpos;
        this->lastY = ypos;
        firstMouse = false;
    }

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
        if(scop->color_mix == 0.0f)
            scop->swap_image(1);
        else if(scop->color_palette_id < 5)
            scop->color_palette_id++;
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    {
        if(scop->color_mix == 0.0f)
            scop->swap_image(-1);
        else if(scop->color_palette_id > 0)
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
    if (key == GLFW_KEY_V && action == GLFW_PRESS)
        inputMode = (inputMode == MOVE_CAMERA ? MOVE_OBJECT: MOVE_CAMERA);
    if (key == GLFW_KEY_X && action == GLFW_PRESS)
    {
        wireframe = !wireframe;
        if (wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    (void)scancode;
    (void)mode;
}

void InputManager::window_focus_callback(GLFWwindow* window, int focused) {
    if (focused == false) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        capturing_mouse = false;
    }
}

void InputManager::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            capturing_mouse = true;
            firstMouse = true;
        }
        if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            capturing_mouse = false;
        }
    }
    (void)mods;
}