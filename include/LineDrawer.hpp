#ifndef LINEDRAWER_HPP
#define LINEDRAWER_HPP

#include <vector>
#include "Shader.hpp"
#include "struct.hpp"

#define GRID_COLOR 0xaaaaaa

//this class will contain lines and draw them
// - add_line: add a line to the list of lines
// - draw: draw all the lines
// - toggle: toggle the visibility of the lines
// - clear: clear all the lines
// - add_axes: add the x, y, z axes
// - add_xgrid(n): add a grid of n lines in the yz plane
// - add_ygrid(n): add a grid of n lines in the xz plane
// - add_zgrid(n): add a grid of n lines in the xy plane
class LineDrawer
{
private:
    //vector of lines
    std::vector<VertexColor> lines_vertex;
    int vertex_size;
    Shader shader;
    GLuint vao, vbo;
    bool toggled = true;
    // VAO vao;
    // VBO vbo;
    void computeBuffer();

public:
    LineDrawer();
    ~LineDrawer();

    //add a line to the list of lines
    void add_line(float x1, float y1, float z1, float x2, float y2, float z2, float r, float g, float b);
    void add_line(Point3D a, Point3D b, int color);
    void add_horizontal_line(Point3D start, float len, int color);
    void add_vertical_line(Point3D start, float len, int color);
    void add_depth_line(Point3D start, float len, int color);

    // void add_line(const Line3D line);

    //draw all the lines
    void draw(const GLfloat *view, const GLfloat *projection);

    //toggle the visibility of the lines
    void toggle();

    //clear all the lines
    void clear();

    //add the x, y, z axes
    void add_axes();

    //add a grid of n lines in the yz plane
    void add_xgrid(int n, int grid_size);

    //add a grid of n lines in the xz plane
    void add_ygrid(int n, int grid_size);

    //add a grid of n lines in the xy plane
    void add_zgrid(int n, int grid_size);
};

#endif