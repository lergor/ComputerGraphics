#pragma once

#include <glm/glm.hpp>
#include "../program/shaders.h"

class Material {

public:

    Material() = default;

    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);

    void set_diffuse_color(glm::vec3 diffuse);

    void set_ambient_color(glm::vec3 ambient);

    void set_specular_color(glm::vec3 specular);

    void set_shininess_color(float shininess);

    glm::vec3 diffuse_color();

    glm::vec3 ambient_color();

    glm::vec3 specular_color();

    float shininess();

    void import_to_shader(Shaders &shader);

private:

    glm::vec3 _diffuse;
    glm::vec3 _ambient;
    glm::vec3 _specular;
    float _shininess;
};

