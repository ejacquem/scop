#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include <vector>
#include "Shader.hpp"

#define SKYBOX_PATH "assets/skybox_space/"

class Skybox
{
private:
    GLuint vao, vbo;
    Shader skyboxShader;
    unsigned int cubemapTexture;

public:
    void draw(mat4 skybox_view, mat4 projection);
    Skybox();
    ~Skybox();
};

#endif