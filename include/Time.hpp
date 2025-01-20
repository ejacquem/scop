#ifndef TIME_HPP
#define TIME_HPP

#include "glfw-3.4/include/GLFW/glfw3.h"
#include <string>
#include <iostream>
#include <iomanip>

class Time
{
private:
    float lastTime = 0.0f;

public:
    inline static float deltaTime = 0.0f;
    inline static float currentTime = 0.0f;

    Time(){}

    ~Time(){}

    void update()
    {
        lastTime = currentTime;
        currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
    }
};

class Timer
{
private:
    float startTime;
    std::string name;

public:

    Timer(){}
    ~Timer(){}

    void start(const std::string& timerName)
    {
        name = timerName;
        startTime = glfwGetTime();
    }
    void stop()
    {
        double elapsedTime = (glfwGetTime() * 1000) - (startTime * 1000);
        std::cout << "Timer " << name << ": " << std::fixed << std::setprecision(6) << elapsedTime << " ms" << std::endl;
    }

};
#endif