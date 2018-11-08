#include "light.h"

Light::Light(glm::vec3 position,
             std::string const &prefix,
             glm::vec3 ambient,
             glm::vec3 diffuse,
             glm::vec3 specular)
        : _position(position)
        , _prefix(prefix)
        , _ambient(ambient)
        , _diffuse(diffuse)
        , _specular(specular)
{
    init_shadow_map();
}

void Light::set_diffuse_color(glm::vec3 diffuse) {
    _diffuse = diffuse;
}

void Light::set_ambient_color(glm::vec3 ambient) {
    _ambient = ambient;
}

void Light::set_specular_color(glm::vec3 specular) {
    _specular = specular;
}

glm::vec3 Light::diffuse_color() {
    return _diffuse;
}

glm::vec3 Light::ambient_color() {
    return _ambient;
}

glm::vec3 Light::specular_color() {
    return _specular;
}

glm::vec3 Light::position() {
    return _position;
}

glm::mat4 Light::light_VP_matrix() {
    float near_plane = 1.0f, far_plane = 7.5f;
    glm::mat4 light_projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    glm::mat4 light_view = glm::lookAt(_position, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    return light_projection * light_view;
}

glm::mat4 Light::depth_bias_VP() {
    const glm::mat4 biasMatrix(
            0.5, 0.0, 0.0, 0.0,
            0.0, 0.5, 0.0, 0.0,
            0.0, 0.0, 0.5, 0.0,
            0.5, 0.5, 0.5, 1.0
    );
    return biasMatrix * light_VP_matrix();
}

void Light::init_shadow_map() {

    glGenFramebuffers(1, &depth_map_FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, depth_map_FBO);

    glGenTextures(1, &depth_map);
    glBindTexture(GL_TEXTURE_2D, depth_map);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

    float borderColor[] = {1.0, 1.0, 1.0, 1.0};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_map, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Light::import_parameters(Shaders &program) {
    program.use();
    program.set_vec3(_prefix + ".ambient", _ambient);
    program.set_vec3(_prefix + ".diffuse", _diffuse);
    program.set_vec3(_prefix + ".specular", _specular);
    program.set_vec3(_prefix + ".position", _position);
    program.set_mat4(_prefix + ".lightSpaceMatrix", light_VP_matrix());
}
