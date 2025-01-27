#include "utils.hpp"

void print_welcome()
{
    std::cout << BRIGHT_YELLOW "\nWelcome to the Scop program!" RESET << std::endl;
    std::cout << "|----- Controls -----" << std::endl;
    std::cout << "|       wasd | camera/object movement" << std::endl;
    std::cout << "|      mouse | camera direction" << std::endl;
    std::cout << "|left, right | change 3D model" << std::endl;
    std::cout << "|   up, down | change color palette" << std::endl;
    std::cout << "|          f | show grid & axis" << std::endl;
    std::cout << "|          c | camera orbit" << std::endl;
    std::cout << "|          t | apply texture" << std::endl;
    std::cout << "|          p | pause rotation" << std::endl;
    std::cout << "|          v | switch to object control" << std::endl;
    std::cout << "|          x | switch wireframe mode" << std::endl;
    std::cout << "|     scroll | change camera speed" << std::endl;
    std::cout << "|ctrl+scroll | change fov" << std::endl;
    std::cout << "|--------------------" << std::endl << std::endl;

    std::vector<std::string> obj_list = get_sorted_file_list("resources/", ".obj");

    std::cout << "Available objects:" << std::endl;
    for (size_t i = 0; i < obj_list.size(); i++)
    {
        std::cout << i << ": " << obj_list[i] << std::endl;
    }
    std::cout << std::endl;
}

// @return A sorted list of object files in the directory with the given extension.
std::vector<std::string> get_sorted_file_list(const std::string& directory, const std::string& extension)
{
    namespace fs = std::filesystem;
    std::vector<std::string> obj_list;

    try {
        if (!fs::exists(directory) || !fs::is_directory(directory)) {
            std::cerr << "Directory does not exist or is not a directory." << std::endl;
            return obj_list;
        }

        for (const auto& entry : fs::directory_iterator(directory)) {
            if (fs::is_regular_file(entry)) {
                if (entry.path().extension() == extension)
                {
                    // std::cout << entry.path().filename() << std::endl;
                    obj_list.push_back(entry.path().filename());
                }
            }
        }
        std::sort(obj_list.begin(), obj_list.end());
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return obj_list;
}

std::string getFileExtension(const std::string& fileName) {
    std::filesystem::path filePath(fileName);
    return filePath.extension().string();
}

// @return The position of the file in the sorted list, or 0 by default.
int get_file_index(const std::string& filename, const std::string& directory)
{
    std::vector<std::string> obj_list = get_sorted_file_list(directory, getFileExtension(filename));

    for (size_t i = 0; i < obj_list.size(); i++)
    {
        if(directory + obj_list[i] == filename)
        {
            return i;
        }
    }
    return 0;
}

//this include is needed here for the stb library for some reason
#include "stb_image.h" //load texture

// Load an image from the given path and return the texture ID.
// @param nb_channel force stbi to use this nb of channel
unsigned int load_image(const char *path, int option1, int nb_channel)
{
    stbi_set_flip_vertically_on_load(true);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // load and generate the texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, option1);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, option1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    int width = 0, height = 0, nrChannels = 0;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, nb_channel);
    if(nb_channel)
        nrChannels = nb_channel;

    if (data)
    {
        std::cout << "loading image: " << path << ", channel nbr: " << nrChannels << std::endl;
        if (nrChannels == 3) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else if (nrChannels == 4) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }
    else
    {
        int b = 0x000000; // black
        int m = 0xff00ff; // magenta
        int default_img[4] = {b,m,m,b};
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, default_img);
        glGenerateMipmap(GL_TEXTURE_2D);
        std::cout << "Failed to load texture: " << path << ", stbi error: " << stbi_failure_reason() << std::endl;
    }
    return texture;
}

unsigned int loadCubemap(std::vector<std::string> faces)
{
    stbi_set_flip_vertically_on_load(false);

    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            if (width != height) {
                std::cout << "Cubemap texture must be square: width " << width << ", height: " << height << " for " << faces[i] << std::endl;
                stbi_image_free(data);
                continue;
            }
            if (nrChannels == 3) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            }
            else if (nrChannels == 4) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            }
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }

    return textureID;
}  