#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Math.hpp"

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// Scroll mode to control speed or fov
enum Camera_Scroll_Mode {
    SPEED_MODE,
    FOV_MODE
};

// Default camera values
#define YAW         -90.0f
#define PITCH        0.0f
#define SPEED        5.0f
#define SENSITIVITY  0.1f
#define FOV          90.0f
#define MIN_FOV      15.0f
#define MAX_FOV      150.0f

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    vec3 position;
    vec3 front;
    vec3 up;
    vec3 right;
    // euler Angles
    float pitch; // up/down
    float yaw;   // left/right
    // camera options
    float speed;
    float sensitivity;
    float fov;
    bool constrainPitch;

    // rotates around the world or not
    bool rotate;
    
    vec3 worldUp = vec3(0.0f, 1.0f, 0.0f);

    // constructor with vectors
    Camera(vec3 pos = vec3(0.0f, 0.0f, 0.0f), vec2 direction = vec2(YAW, PITCH))
    {
        position = pos;
        yaw = direction.x;
        pitch = direction.y;
        fov = FOV;
        speed = SPEED;
        sensitivity = SENSITIVITY;
        constrainPitch = true;

        rotate = false;

        updateCameraVectors();
    }

    Camera& setDirection(vec2 direction)
    {
        pitch = direction.y;
        yaw = direction.x;
        updateCameraVectors();
        return *this;
    }

    Camera& setPitch(float pitch)
    {
        this->pitch = pitch;
        return *this;
    }

    Camera& setYaw(float yaw)
    {
        this->yaw = yaw;
        return *this;
    }

    Camera& setPosition(vec3 position)
    {
        this->position = position;
        return *this;
    }

    Camera& setSpeed(float speed)
    {
        this->speed = speed;
        return *this;
    }

    Camera& setSensitivity(float sensitivity)
    {
        this->sensitivity = sensitivity;
        return *this;
    }

    Camera& setFov(float fov)
    {
        this->fov = fov;
        return *this;
    }

    void lookAt(vec3 pos)
    {
        front = normalize(pos - position);
        yaw = degrees(atan2(front.z, front.x));
        pitch = degrees(asin(front.y));
        updateCameraVectors();
    }

    // rotates around the center of the world
    void rotateAroundCenter(float deltaTime)
    {
        if (rotate == false)
            return;
        lookAt(vec3(0.0f, 0.0f, 0.0f));
        float velocity = speed * deltaTime;
        position += right * velocity;
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    mat4 GetViewMatrix()
    {
        return ::lookAt(position, position + front, up);
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void processKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = speed * deltaTime;
        vec3 front_no_y = normalize(vec3(front.x, 0, front.z));
        switch (direction)
        {
            case FORWARD:
                position += front_no_y * velocity; break;
            case BACKWARD:
                position -= front_no_y * velocity; break;
            case RIGHT:
                position += right * velocity; break;
            case LEFT:
                position -= right * velocity; break;
            case UP:
                position += worldUp * velocity; break; // depends on worldUp to follow z axis
            case DOWN:
                position -= worldUp * velocity; break;
        }
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void processMouseMovement(float xoffset, float yoffset)
    {
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw   += xoffset;
        pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (pitch > 85.0f)
                pitch = 85.0f;
            if (pitch < -85.0f)
                pitch = -85.0f;
        }

        // update front, right and up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void processMouseScroll(float yoffset, Camera_Scroll_Mode mode)
    {
        if (mode == SPEED_MODE)
        {
            speed *= pow(1.5, yoffset);
            if(speed < 0.01)
                speed = 0.01;
        }
        if (mode == FOV_MODE)
        {
            fov -= (float)(yoffset * 2);
            if (fov < MIN_FOV)
                fov = MIN_FOV;
            if (fov > MAX_FOV)
                fov = MAX_FOV;
        }
    }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the new front vector
        front.x = cos(radians(yaw)) * cos(radians(pitch));
        front.y = sin(radians(pitch));
        front.z = sin(radians(yaw)) * cos(radians(pitch));
        front = normalize(front);
        // also re-calculate the right and up vector
        right = normalize(cross(front, worldUp)); // cross front with up to get right
        up    = normalize(cross(right, front)); // cross right with front to get up
    }
};
#endif

