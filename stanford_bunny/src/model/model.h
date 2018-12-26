#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>

#include "mesh.h"
#include "../lightning/material.h"

class Model {

public:

    Model() = default;

    explicit Model(const char *path,
                   glm::mat4 model_matrix = glm::mat4(0.1f),
                   Material material = Material(glm::vec3(0.1f), glm::vec3(0.5f), glm::vec3(0.5f), 32.0f),
                   bool rotating = false);

    void draw();

    void clear();

    glm::mat4 matrix();

    Material& material();

    void import_parameters(Shaders &shader);

private:

    std::vector<Mesh> _meshes;
    glm::mat4 _model_matrix;
    Material _material;
    float _last_time = 0.0f;
    bool _rotating;
    glm::vec3 _orientation;

    void load_model(const std::string &path);

    void process_node(aiNode *node, const aiScene *scene);

    Mesh process_mesh(aiMesh *mesh);

    void rotate();

};