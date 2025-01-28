#include "ObjectLoader.hpp"
#include <algorithm>
#include <iostream>
#include <cstring>

// for mmap:
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

ObjectLoader::ObjectLoader() {}
ObjectLoader::~ObjectLoader() {}

std::string readFile(const std::string &filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        throw std::runtime_error("Unable to open file: " + filePath);
    }
    std::ostringstream contentStream;
    contentStream << file.rdbuf();
    return contentStream.str();
}

void ObjectLoader::reset()
{
    vertices_buffer.clear();
    indices_buffer.clear();

    maxx = maxy = maxz = -1000000.0f;
    minx = miny = minz = +1000000.0f;
}

Object* ObjectLoader::parse(const std::string &filePath)
{
    reset();

    // std::cout << "parsing: " << filePath << std::endl;
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cout << "Unable to open file: " << filePath << std::endl;
        return NULL;
    }

    // Timer timer;
    // timer.start("object parsing");
    char line_buffer[1024];
    while (file.getline(line_buffer, sizeof(line_buffer)))
    {
        this->parseLine(line_buffer);
    }
    std::cout << vertices_buffer.size() << " vertices, " <<  indices_buffer.size() << " indices" <<std::endl;
    // timer.stop();

    float radius = (maxy - miny) / 2;

    for (auto &v : vertices_buffer)
    {
        // align object to center
        v.x -= (maxx + minx) / 2;
        v.y -= (maxy + miny) / 2;
        v.z -= (maxz + minz) / 2;

        v.u = 0.5 - atan2(v.z, v.x) / (2 * M_PI);
        v.v = (0.5 + asin(v.y / radius) / M_PI);
    }

    fixSeamTexture();

    // timer.start("creating object buffer");
    Object* a = new Object(vertices_buffer, indices_buffer);
    // timer.stop();
    return a;
}

void ObjectLoader::parseIndice(const char *line)
{
    int i, j, k;
    if((line = strchr(line, ' ')))
        i = atoi(line);
    if((line = strchr(line + 1, ' ')))
        j = atoi(line);
    while((line = strchr(line + 1, ' ')))
    {
        k = atoi(line);
        int max_vertex_id = vertices_buffer.size();
        // check if id or indices is valid so Alex can stop crashing my scop
        if (i <= max_vertex_id && j <= max_vertex_id && k <= max_vertex_id)
            indices_buffer.push_back({i - 1, j - 1, k - 1});
        j = k;
    }
}

void ObjectLoader::parseLine(const char *line)
{
    float x = 0, y = 0, z = 0;

    // example:
    // v -0.500000 -0.500000 -0.500000
    if (line[0] == 'v' && line[1] == ' ')
    {
        char* ptr = (char *)line + 2;
        x = strtof(ptr, &ptr);
        y = strtof(ptr, &ptr);
        z = strtof(ptr, &ptr);
        vertices_buffer.push_back({x, y, z, 0, 0});

        maxx = std::max(maxx, x);
        minx = std::min(minx, x);
        maxy = std::max(maxy, y);
        miny = std::min(miny, y);
        maxz = std::max(maxz, z);
        minz = std::min(minz, z);
    }
    // f 1/a/b 2// 3 4 5 turns into 1 2 3 | 1 3 4 | 1 4 5
    else if (line[0] == 'f' && line[1] == ' ')
    {
        parseIndice(line);
    }
}

void ObjectLoader::fixSeamTexture()
{
    for (auto &i : indices_buffer)
    {
        float u1 = vertices_buffer[i.x].u;
        float u2 = vertices_buffer[i.y].u;
        float u3 = vertices_buffer[i.z].u;

        bool is_one_vertex_near_0_0 = u1 <= 0.1 || u2 <= 0.1 || u3 <= 0.1;
        bool is_one_vertex_near_0_9 = u1 > 0.8 || u2 > 0.8 || u3 > 0.8;

        if (is_one_vertex_near_0_0 && is_one_vertex_near_0_9)
        {
            Vertextex new_vertex;
            //indice: 1 5 9 -> 1 point to a vertex with u==0, so we duplicate that vertex x with u=1 and assign indice to: x 5 9
            if(u1 <= 0.1)
            {
                new_vertex = vertices_buffer[i.x];
                new_vertex.u += 1;
                i.x = vertices_buffer.size();
                vertices_buffer.push_back(new_vertex);
            }
            if(u2 <= 0.1)
            {
                new_vertex = vertices_buffer[i.y];
                new_vertex.u += 1;
                i.y = vertices_buffer.size();
                vertices_buffer.push_back(new_vertex);
            }
            if(u3 <= 0.1)
            {
                new_vertex = vertices_buffer[i.z];
                new_vertex.u += 1;
                i.z = vertices_buffer.size();
                vertices_buffer.push_back(new_vertex);
            }
        }
    }
}
