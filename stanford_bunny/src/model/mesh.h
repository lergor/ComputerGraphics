#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "vertex.h"

class Mesh {
public:

    Mesh() = default;

    Mesh(std::vector<Vertex> const &vertices, std::vector<GLuint> const &indices);

    void draw();

    void delete_mesh();

private:

    std::vector<Vertex> _vertices;
    std::vector<GLuint> _indices;

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    void setup();
};
