#include "load_shaders.h"

namespace {

    void check_info_log(GLuint id) {
        int log_len;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &log_len);
        if (log_len > 0) {
            std::vector<char> error_message(static_cast<unsigned long>(log_len + 1));
            glGetProgramInfoLog(id, log_len, nullptr, &error_message[0]);
            printf("%s\n", &error_message[0]);
        }
    }

    void read_and_compile_shader(const char *path, GLint id) {
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

}

GLuint load_shaders(const char *vertex_file_path, const char *fragment_file_path) {

    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    read_and_compile_shader(vertex_file_path, vertex_shader_id);
    read_and_compile_shader(fragment_file_path, fragment_shader_id);


    GLuint program_ID = glCreateProgram();
    glAttachShader(program_ID, vertex_shader_id);
    glAttachShader(program_ID, fragment_shader_id);

    printf("Linking program\n");
    glLinkProgram(program_ID);
    check_info_log(program_ID);

    glDetachShader(program_ID, vertex_shader_id);
    glDetachShader(program_ID, fragment_shader_id);

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);
    return program_ID;
}
