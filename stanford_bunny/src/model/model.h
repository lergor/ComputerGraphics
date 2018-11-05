#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>

#include "mesh.h"

class Model {

public:

    Model();

    explicit Model(const char *path, glm::mat4 model_matrix = glm::mat4(0.1f));

    void draw();

    void delete_model();

    glm::mat4 matrix();

private:

    std::vector<Mesh> _meshes;
    glm::mat4 _model_matrix;

    void load_model(const std::string &path);

    void process_node(aiNode *node, const aiScene *scene);

    Mesh process_mesh(aiMesh *mesh);
};