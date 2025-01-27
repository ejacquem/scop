#include "Scop.hpp"

Scop::Scop()
{
    obj_index = -1;
    color_palette_id = 0;
    color_mix = 0;
    color_mix_speed = -1;
    rotation_speed = Scop::DEFAULT_SPEED;
    object = nullptr;
    current_texture_id = 0;
    position = vec3(0.0f);
    direction = vec3(0.0f);
    move_speed = 2;

    textures[0] = load_image("assets/goodman.jpg", GL_REPEAT);
    textures[1] = load_image("assets/earth.jpg", GL_REPEAT);
    textures[2] = load_image("assets/skybox_space/back.png", GL_REPEAT);
    textures[3] = load_image("assets/nyancat.png", GL_REPEAT);
    textures[4] = load_image("assets/kitten.jpg", GL_REPEAT);
    textures[5] = load_image("assets/kitten2.jpg", GL_REPEAT);
    textures[6] = load_image("assets/cutecat.png", GL_REPEAT, 4);
    textures[7] = load_image("undefined", GL_REPEAT);
    textures_nbr = 8;
    // textures[0] = load_image("undefined", GL_REPEAT);
    // textures_nbr = 1;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
}

Scop::~Scop()
{
    if (object)
        delete object;
}

void Scop::swap(int direction)
{
    std::vector<std::string> obj_list = get_sorted_file_list(OBJ_PATH, ".obj");

    int len = obj_list.size();
    
    obj_index += direction;
    obj_index = (obj_index + len) % len;

    load(OBJ_PATH + obj_list[obj_index]);
}

void Scop::swap_image(int direction)
{
    current_texture_id = (current_texture_id + direction + textures_nbr) % textures_nbr;
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[current_texture_id]);
}

void Scop::draw(const GLfloat *model, const GLfloat *view, const GLfloat *projection)
{
    if(object == NULL)
        return;
    shader->use();
    (*(mat4 *)model)[3] = vec4(position.x, position.y, position.z, 1);
    shader->setMat4("model", model);
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);

    shader->setInt("color_palette_id", color_palette_id);

    color_mix += color_mix_speed * Time::deltaTime;
    if(color_mix < 0)
        color_mix = 0;
    if(color_mix > 1)
        color_mix = 1;

    shader->setFloat("color_mix", color_mix);

    object->draw();
}

void Scop::load(const std::string &filename)
{
    std::cout << "\nloading: " << filename << std::endl;
    if (obj_index == -1)
    {
        obj_index = get_file_index(filename, OBJ_PATH);
        std::cout << "obj_index: " << obj_index << std::endl;
    }
    if (object)
        delete object;
    try
    {
        object = objLoader.parse(filename);
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error while loading the object: " << e.what() << std::endl;
        if (object)
            delete object;
    }
    
}

void Scop::setShader(Shader *shader)
{
    this->shader = shader;
}

void Scop::move()
{
    position.x += direction.x * Time::deltaTime;
    position.y += direction.y * Time::deltaTime;
    position.z += direction.z * Time::deltaTime;
}

