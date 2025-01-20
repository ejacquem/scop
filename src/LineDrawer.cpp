#include "LineDrawer.hpp"

LineDrawer::LineDrawer() :
shader("shaders/3d_line.vert", "shaders/3d_line.frag")
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);  // Initially empty

    // Set up position attributes (x1, y1, z1, x2, y2, z2)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Set up color attributes int (color)                          //offset of 3 floats
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    vertex_size = 0;
}

LineDrawer::~LineDrawer()
{
    glDeleteBuffers(1, &vbo);
}

//add a line to the list of lines
void LineDrawer::add_line(float x1, float y1, float z1, float x2, float y2, float z2, float r, float g, float b) {
    lines_vertex.push_back({x1, y1, z1, r, g, b});
    lines_vertex.push_back({x2, y2, z2, r, g, b});
    this->computeBuffer();
}

void LineDrawer::add_line(Point3D i, Point3D j, int color)
{
    float r = color & 0xFF;
    float g = (color >> 8) & 0xFF;
    float b = (color >> 16) & 0xFF;
    lines_vertex.push_back({i.x, i.y, i.z, r, g, b});
    lines_vertex.push_back({j.x, j.y, j.z, r, g, b});
    this->computeBuffer();
}

void LineDrawer::add_horizontal_line(Point3D start, float len, int color)
{
    Point3D end = {start.x + len, start.y, start.z};
    this->add_line(start, end, color);
}

void LineDrawer::add_vertical_line(Point3D start, float len, int color)
{
    Point3D end = {start.x, start.y + len, start.z};
    this->add_line(start, end, color);
}

void LineDrawer::add_depth_line(Point3D start, float len, int color)
{
    Point3D end = {start.x, start.y, start.z + len};
    this->add_line(start, end, color);
}


//draw all the lines
void LineDrawer::draw(const GLfloat *view, const GLfloat *projection)
{
    if (toggled == false) return;
    if (vertex_size == 0) return;

    shader.use();
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, vertex_size);

    glBindVertexArray(0);
}

//update the buffer object size
void LineDrawer::computeBuffer()
{
    vertex_size = lines_vertex.size();

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertex_size * sizeof(VertexColor), lines_vertex.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
    glBindVertexArray(0);
}

//toggle the visibility of the lines
void LineDrawer::toggle()
{
    toggled = !toggled;
}

//clear all the lines
void LineDrawer::clear(){}

//add the x, y, z axes
void LineDrawer::add_axes()
{
    float len = 2.0f;
    float arrow_len = 0.1f;
    float letter_offset = 0.1f;
    // float neg = 0;
    
    Point3D origin = {0.001f, 0.001f, 0.0f};
    Point3D x_end = {len, 0.0f, 0.0f};
    Point3D y_end = {0.0f, len, 0.0f};
    Point3D z_end = {0.0f, 0.0f, len};

    //axis
    this->add_line(origin, x_end, 0xFF0000);
    this->add_line(origin, y_end, 0x00FF00);
    this->add_line(origin, z_end, 0x0000FF);

    //x arrow
    this->add_line(x_end, {(len - arrow_len), arrow_len, 0.0f}, 0xFF0000);
    this->add_line(x_end, {(len - arrow_len), -arrow_len, 0.0f}, 0xFF0000);
    //x symbol
    x_end.x += letter_offset;
    this->add_line({(x_end.x),  arrow_len, 0.0f}, {(x_end.x + arrow_len), -arrow_len, 0.0f}, 0xFF0000);
    this->add_line({(x_end.x), -arrow_len, 0.0f}, {(x_end.x + arrow_len),  arrow_len, 0.0f}, 0xFF0000);

    //y arrow
    this->add_line(y_end, { arrow_len, (len - arrow_len), 0.0f}, 0x00FF00);
    this->add_line(y_end, {-arrow_len, (len - arrow_len), 0.0f}, 0x00FF00);
    //y symbol
    y_end.y += letter_offset;
    this->add_line({-arrow_len/2, (y_end.y + arrow_len * 2), 0.0f}, { 0.0f, (y_end.y + arrow_len), 0.0f}, 0x00FF00);
    this->add_line({-arrow_len/2, (y_end.y), 0.0f}, { arrow_len/2, (y_end.y + arrow_len * 2), 0.0f}, 0x00FF00);

    //z arrow
    this->add_line(z_end, {0.0f, arrow_len, (len - arrow_len)}, 0x0000FF);
    this->add_line(z_end, {0.0f, -arrow_len, (len - arrow_len)}, 0x0000FF);
    //z symbol
    Point3D rtop = {0.0f,  arrow_len, len + letter_offset};
    Point3D rbot = {0.0f, -arrow_len, len + letter_offset};
    Point3D ltop = {0.0f,  arrow_len, len + letter_offset + arrow_len};
    Point3D lbot = {0.0f, -arrow_len, len + letter_offset + arrow_len};
    this->add_line(ltop, rtop, 0x0000FF);
    this->add_line(lbot, rtop, 0x0000FF);
    this->add_line(lbot, rbot, 0x0000FF);
}

//add a grid of n lines in the yz plane
void LineDrawer::add_xgrid(int n, int grid_size)
{
    float len = n * grid_size;

    Point3D starty = {0, -len, 0};
    Point3D startz = {0, 0, -len};
    for (int i = -n; i <= n; i++)
    {
        startz.y = i * grid_size;
        starty.z = i * grid_size;
        this->add_vertical_line(starty, len * 2, GRID_COLOR);
        this->add_depth_line(startz, len * 2, GRID_COLOR);
    }
}

//add a grid of n lines in the xz plane
void LineDrawer::add_ygrid(int n, int grid_size)
{
    float len = n * grid_size;

    Point3D startx = {-len, 0, 0};
    Point3D startz = {0, 0, -len};
    for (int i = -n; i <= n; i++)
    {
        startz.x = i * grid_size;
        startx.z = i * grid_size;
        this->add_horizontal_line(startx, len * 2, GRID_COLOR);
        this->add_depth_line(startz, len * 2, GRID_COLOR);
    }
}

//add a grid of n lines in the xy plane
void LineDrawer::add_zgrid(int n, int grid_size)
{
    float len = n * grid_size;

    Point3D startx = {-len, 0, 0};
    Point3D starty = {0, -len, 0};
    for (int i = -n; i <= n; i++)
    {
        startx.y = i * grid_size;
        starty.x = i * grid_size;
        this->add_horizontal_line(startx, len * 2, GRID_COLOR);
        this->add_vertical_line(starty, len * 2, GRID_COLOR);
    }
}
