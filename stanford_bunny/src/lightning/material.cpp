#include "material.h"

Material::Material(glm::vec3 ambient, glm::vec3 diffuse,
                   glm::vec3 specular, float shininess = 32.0f)
        : _ambient(ambient)
        , _diffuse(diffuse)
        , _specular(specular)
        , _shininess(shininess)
{}

void Material::set_diffuse_color(glm::vec3 diffuse) {
    _diffuse = diffuse;
}

void Material::set_ambient_color(glm::vec3 ambient) {
    _ambient = ambient;
}

void Material::set_specular_color(glm::vec3 specular) {
    _specular = specular;
}

void Material::set_shininess_color(float shininess) {
    _shininess = shininess;
}

glm::vec3 Material::diffuse_color() {
    return _diffuse;
}

glm::vec3 Material::ambient_color() {
    return _ambient;
}

glm::vec3 Material::specular_color() {
    return _specular;
}

float Material::shininess() {
    return _shininess;
}

void Material::import_to_shader(Shaders &shader) {
    shader.set_vec3("material.ambient", _ambient);
    shader.set_vec3("material.diffuse", _diffuse);
    shader.set_vec3("material.specular", _specular);
    shader.set_float("material.shininess", _shininess);
}


