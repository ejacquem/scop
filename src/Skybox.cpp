#include "Skybox.hpp"
#include "utils.hpp"

Skybox::Skybox() :
skyboxShader("shaders/skybox.vert", "shaders/skybox.frag")
{
    std::vector<std::string> faces
    {
        SKYBOX_PATH "right.png",  // POSITIVE_X
        SKYBOX_PATH "left.png",   // NEGATIVE_X
        SKYBOX_PATH "top.png",    // POSITIVE_Y
        SKYBOX_PATH "bottom.png", // NEGATIVE_Y
        SKYBOX_PATH "front.png",  // POSITIVE_Z
        SKYBOX_PATH "back.png"    // NEGATIVE_Z
    };
    cubemapTexture = loadCubemap(faces);  

    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

Skybox::~Skybox(){}

void Skybox::draw(mat4 skybox_view, mat4 projection)
{
    skybox_view[3][0] = 0;
    skybox_view[3][1] = 0;
    skybox_view[3][2] = 0;
    glDepthFunc(GL_LEQUAL);
    skyboxShader.use();

    skyboxShader.setInt("skybox", 0);
    skyboxShader.setMat4("view", (const GLfloat *)&skybox_view);
    skyboxShader.setMat4("projection", (const GLfloat *)&projection);
    // glUniform1i(glGetUniformLocation(skyboxShader.ID, "skybox"), 0);

    // ... set view and projection matrix
    glBindVertexArray(vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default
}