#ifndef SCOP_HPP
#define SCOP_HPP

#include "Camera.hpp"
#include "Shader.hpp"
#include "Object.hpp"
#include "ObjectLoader.hpp"
#include "LineDrawer.hpp"
#include "utils.hpp"

#define OBJ_PATH "resources/"

class Scop
{

public:
    float color_mix_speed;
    int color_palette_id;
    float color_mix;
    float rotation_speed; // deg / sec

    inline float static const DEFAULT_SPEED = 1;

    Scop();
    ~Scop();

    void swap(int direction);
    void draw(const GLfloat *model, const GLfloat *view, const GLfloat *projection);
    void load(const std::string &filename);
    void setShader(Shader *shader);

private:
    Shader *shader;
    Object *object;
    ObjectLoader objLoader;

    int obj_index;
};

#endif