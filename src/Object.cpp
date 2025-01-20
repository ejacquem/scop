#include "Object.hpp"

Object::~Object()
{
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

Object::Object(std::vector<Vertextex> &vertices, std::vector<Indice> &indices): 
vertices(vertices), 
indices(indices)
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertextex), this->vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(Indice), this->indices.data(), GL_STATIC_DRAW);

    // Set up position attributes (x, y, z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertextex), (void*)offsetof(Vertextex, x));
    glEnableVertexAttribArray(0);

    // Set up UV attribute (u, v)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertextex), (void*)offsetof(Vertextex, u));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Object::draw()
{
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size() * 3, GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
}
