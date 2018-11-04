#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/detail/type_mat4x4.hpp>

class Shaders {

public:

    Shaders(const char *vertex_shader, const char *fragment_shader);

    void use();

    GLuint program_ID();

    void set_bool(const std::string &name, bool value) const;

    void set_int(const std::string &name, int value) const;

    void set_float(const std::string &name, float value) const;

    void set_vec2(const std::string &name, const glm::vec2 &value) const;

    void set_vec2(const std::string &name, float x, float y) const;

    void set_vec3(const std::string &name, const glm::vec3 &value) const;

    void setVec3(const std::string &name, float x, float y, float z) const;

    void set_vec4(const std::string &name, const glm::vec4 &value) const;

    void set_vec4(const std::string &name, float x, float y, float z, float w);

    void set_mat2(const std::string &name, const glm::mat2 &mat) const;

    void set_mat3(const std::string &name, const glm::mat3 &mat) const;

    void set_mat4(const std::string &name, const glm::mat4 &mat) const;

    void delete_program();

private:

    GLuint _vertex_shader_ID;
    GLuint _fragment_shader_ID;
    GLuint _program_ID;

    void read_and_compile_shader(const char *path, GLuint id);

    void check_info_log(GLuint id);
};

