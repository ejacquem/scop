#ifndef STRUCT_HPP
#define STRUCT_HPP

struct Vertex
{
    float x, y, z;
};

//vertex that store a texture
struct Vertextex
{
    float x, y, z;
    float u, v;
};

struct Indice
{
    int x, y, z;
};

struct VertexColor
{
    float x, y, z;
    float r, g, b;
};

struct Point3D
{
    float x, y, z;
};

#endif