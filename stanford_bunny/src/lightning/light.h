#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include "../program/shaders.h"

class Light {
public:

    Light() = default;

    explicit Light(glm::vec3 position,
                   int index = 0,
                   GLenum texture = GL_TEXTURE0,
                   glm::vec3 ambient = glm::vec3(0.05f),
                   glm::vec3 diffuse = glm::vec3(1.0f),
                   glm::vec3 specular = glm::vec3(1.0f));

    void set_diffuse_color(glm::vec3 diffuse);

    void set_ambient_color(glm::vec3 ambient);

    void set_specular_color(glm::vec3 specular);

    glm::vec3 diffuse_color();

    glm::vec3 ambient_color();

    glm::vec3 specular_color();

    glm::vec3 position();

    glm::mat4 light_VP_matrix();

    glm::mat4 depth_bias_VP();

    void import_parameters(Shaders &program);

    GLuint depth_map_FBO();

    GLuint depth_map();

    GLenum texture_unit();

private:

    GLuint _depth_map_FBO = 0;
    GLuint _depth_map = 0;
    GLenum _texture = 0;

    glm::vec3 _diffuse;
    glm::vec3 _ambient;
    glm::vec3 _specular;
    glm::vec3 _position;
    std::string _prefix;
    int _index;

    void init_shadow_map();
};
