#include "model.h"

Model::Model(const char *path, glm::mat4 model_matrix)
    : _model_matrix(model_matrix)
{
    load_model(path);
}

void Model::draw() {
    for (auto &mesh : _meshes) {
        mesh.draw();
    }
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

void Model::delete_model() {
    for (auto &mesh : _meshes) {
        mesh.delete_mesh();
    }
}

glm::mat4 Model::matrix() {
    return _model_matrix;
}
