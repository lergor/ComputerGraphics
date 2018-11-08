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
                   std::string const &prefix = "light",
                   glm::vec3 ambient = glm::vec3(0.05f),
                   glm::vec3 diffuse = glm::vec3(1.0f),
                   glm::vec3 specular = glm::vec3(1.0f));

    void set_shadow_depth_program(Shaders *shadow_depth_program) {
        this->shadow_depth_program = shadow_depth_program;
    }


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

    GLuint depth_map_FBO;

    GLuint depth_map;
private:

    Shaders *shadow_depth_program;

    glm::vec3 _diffuse;
    glm::vec3 _ambient;
    glm::vec3 _specular;
    glm::vec3 _position;
    std::string _prefix;

    void init_shadow_map();
};
