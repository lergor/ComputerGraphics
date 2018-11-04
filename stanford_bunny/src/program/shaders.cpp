#include "shaders.h"

Shaders::Shaders(const char *vertex_shader, const char *fragment_shader) {
    _vertex_shader_ID = glCreateShader(GL_VERTEX_SHADER);
    _fragment_shader_ID = glCreateShader(GL_FRAGMENT_SHADER);

    read_and_compile_shader(vertex_shader, _vertex_shader_ID);
    read_and_compile_shader(fragment_shader, _fragment_shader_ID);

    _program_ID = glCreateProgram();
    glAttachShader(_program_ID, _vertex_shader_ID);
    glAttachShader(_program_ID, _fragment_shader_ID);

    printf("Linking program\n");
    glLinkProgram(_program_ID);
    check_info_log(_program_ID);

    glDetachShader(_program_ID, _vertex_shader_ID);
    glDetachShader(_program_ID, _fragment_shader_ID);

    glDeleteShader(_vertex_shader_ID);
    glDeleteShader(_fragment_shader_ID);
}

void Shaders::use() {
    glUseProgram(_program_ID);
}

GLuint Shaders::program_ID() {
    return _program_ID;
}

void Shaders::delete_program() {
    glDeleteProgram(_program_ID);
}

void Shaders::read_and_compile_shader(const char *path, GLuint id) {
    printf("Compiling shader : %s\n", path);
    FILE *file;
    if (!(file = fopen(path, "r"))) {
        fprintf(stderr, "Failed to open %s\n", path);
        return;
    }
    char *buf;
    fseek(file, 0, SEEK_END);
    auto len = static_cast<size_t>(ftell(file));
    fseek(file, 0, SEEK_SET);
    buf = static_cast<char *>(malloc(len + 1));
    fread(buf, 1, len, file);
    buf[len] = 0;

    glShaderSource(id, 1, reinterpret_cast<const GLchar *const *>(&buf), nullptr);
    glCompileShader(id);
    check_info_log(id);
    free((void *) buf);
}

void Shaders::check_info_log(GLuint id) {
    int log_len;
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &log_len);
    if (log_len > 0) {
        std::vector<char> error_message(static_cast<unsigned long>(log_len + 1));
        glGetProgramInfoLog(id, log_len, nullptr, &error_message[0]);
        printf("%s\n", &error_message[0]);
    }
}


void Shaders::set_bool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(_program_ID, name.c_str()), (int) value);
}

void Shaders::set_int(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(_program_ID, name.c_str()), value);
}

void Shaders::set_float(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(_program_ID, name.c_str()), value);
}

void Shaders::set_vec2(const std::string &name, const glm::vec2 &value) const {
    glUniform2fv(glGetUniformLocation(_program_ID, name.c_str()), 1, &value[0]);
}

void Shaders::set_vec2(const std::string &name, float x, float y) const {
    glUniform2f(glGetUniformLocation(_program_ID, name.c_str()), x, y);
}

void Shaders::set_vec3(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(_program_ID, name.c_str()), 1, &value[0]);
}

void Shaders::setVec3(const std::string &name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(_program_ID, name.c_str()), x, y, z);
}

void Shaders::set_vec4(const std::string &name, const glm::vec4 &value) const {
    glUniform4fv(glGetUniformLocation(_program_ID, name.c_str()), 1, &value[0]);
}

void Shaders::set_vec4(const std::string &name, float x, float y, float z, float w) {
    glUniform4f(glGetUniformLocation(_program_ID, name.c_str()), x, y, z, w);
}

void Shaders::set_mat2(const std::string &name, const glm::mat2 &mat) const {
    glUniformMatrix2fv(glGetUniformLocation(_program_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shaders::set_mat3(const std::string &name, const glm::mat3 &mat) const {
    glUniformMatrix3fv(glGetUniformLocation(_program_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shaders::set_mat4(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(_program_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

