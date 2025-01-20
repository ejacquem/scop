#include "utils.hpp"

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
int load_image(const char *path, int option1)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // load and generate the texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, option1);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, option1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    int width = 0, height = 0, nrChannels = 0;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data)
    {
        std::cout << "Loaded texture: " << path << std::endl;
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
        std::cout << "Failed to load texture: " << path << " nbr channel: " << nrChannels  << std::endl;
        std::cout << "stbi error: " << stbi_failure_reason() << std::endl;
    }
    return texture;
}