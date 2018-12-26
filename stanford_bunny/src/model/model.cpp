#include "model.h"

Model::Model(const char *path, glm::mat4 model_matrix, Material material, bool rotating)
        : _model_matrix(model_matrix), _material(material), _rotating(rotating) {
    load_model(path);
}

void Model::draw() {
    for (auto &mesh : _meshes) {
        mesh.draw();
    }
    if (_rotating)
        rotate();
}

void Model::load_model(const std::string &path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path,
                                             aiProcess_Triangulate |
                                             aiProcess_FlipUVs |
                                             aiProcess_GenNormals);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        printf("%s\n", importer.GetErrorString());
        return;
    }
    process_node(scene->mRootNode, scene);
}

void Model::process_node(aiNode *node, const aiScene *scene) {
    for (size_t i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        _meshes.push_back(process_mesh(mesh));
    }
    for (size_t i = 0; i < node->mNumChildren; i++) {
        process_node(node->mChildren[i], scene);
    }
}

Mesh Model::process_mesh(aiMesh *mesh) {
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex{};
        vertex.position = {
                mesh->mVertices[i].x,
                mesh->mVertices[i].y,
                mesh->mVertices[i].z
        };
        vertex.normal = {
                mesh->mNormals[i].x,
                mesh->mNormals[i].y,
                mesh->mNormals[i].z
        };
        vertices.push_back(vertex);
    }

    for (size_t i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (size_t j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
    return Mesh(vertices, indices);
}

void Model::clear() {
    for (auto &mesh : _meshes) {
        mesh.delete_mesh();
    }
}

glm::mat4 Model::matrix() {
    return _model_matrix;
}

Material &Model::material() {
    return _material;
}

void Model::import_parameters(Shaders &shader) {
    _material.import_to_shader(shader);
    shader.set_mat4("model", _model_matrix);
}

void Model::rotate() {
    auto current_time = static_cast<float>(glfwGetTime());
    float delta_time = current_time - _last_time;
    _last_time = current_time;
    _orientation.y = 3.14159f / 2.0f * delta_time;
    glm::mat4 rotation_mat = glm::eulerAngleYXZ(_orientation.y, _orientation.x, _orientation.z);
    _model_matrix = rotation_mat * _model_matrix;
}
