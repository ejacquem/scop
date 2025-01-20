#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <vector>
#include "Shader.hpp"
#include "struct.hpp"

// template <typename T>
class Object
{
private:
    GLuint vao, vbo, ebo;
    std::vector<Vertextex> &vertices;
    std::vector<Indice> &indices;

public:
    void draw();
    Object(std::vector<Vertextex> &vertices, std::vector<Indice> &indices);
    ~Object();
};

#endif