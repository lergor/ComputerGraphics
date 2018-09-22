#include "fractal.h"

namespace {

    GLuint get_texture(int length) {
        auto colors = new GLfloat[length];
        for (int i = 0; i < length; ++i) {
            colors[i] = GLfloat(i) / length;
        }
        GLuint texture_ID;
        glGenTextures(1, &texture_ID);
        glBindTexture(GL_TEXTURE_1D, texture_ID);
        glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, length, 0, GL_BGR, GL_FLOAT, colors);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_1D);
        delete[] colors;
        return texture_ID;
    }

    glm::mat4 calculate_MVP_matrix(GLuint program_ID) {
        glm::mat4 Projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 100.0f);
        glm::mat4 View = glm::lookAt(
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f)
        );
        glm::mat4 Model = glm::mat4(1.0f);
        return Projection * View * Model;
    }

}

Fractal::Fractal(int window_width, int window_height, int max_iter, float max_radius)
        : window_width(window_width)
        , window_height(window_height)
        , max_iter(max_iter)
        , max_radius(max_radius)
{}


void Fractal::init() {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        glfwTerminate();
        return;
    }

    window = Window::get_instance(window_width, window_height);
    glewExperimental = static_cast<GLboolean>(true);

    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        glfwTerminate();
        return;
    }
}

void Fractal::show() {
    init();
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    GLuint vertex_array_ID;
    glGenVertexArrays(1, &vertex_array_ID);
    glBindVertexArray(vertex_array_ID);

    GLuint program_ID = load_shaders("./src/fractal.vert", "./src/fractal.frag");

    GLint matrix_ID = glGetUniformLocation(program_ID, "MVP_matrix");
    GLint scale_ID = glGetUniformLocation(program_ID, "scale");
    GLint shift_ID = glGetUniformLocation(program_ID, "shift");

    glm::mat4 MVP_matrix = calculate_MVP_matrix(program_ID);
    calculate_vertices();

    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, 2 * vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

    GLuint texture = get_texture(1024);
    GLint texture_ID = glGetUniformLocation(program_ID, "sampler");
    GLint max_iter_ID = glGetUniformLocation(program_ID, "max_iter");
    GLint max_radius_ID = glGetUniformLocation(program_ID, "max_radius");

    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program_ID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_1D, texture);
        glUniformMatrix4fv(matrix_ID, 1, GL_FALSE, &MVP_matrix[0][0]);
        glUniform1i(max_iter_ID, max_iter);
        glUniform1f(max_radius_ID, max_radius);

        glm::vec2 scale_vec = window->get_scale();
        glUniform2fv(scale_ID, 1, &scale_vec[0]);

        glm::vec2 shift = window->get_shift();
        glUniform2fv(shift_ID, 1, &shift[0]);

        glUniform1i(texture_ID, 0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
        glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(vertices.size()));
        glDisableVertexAttribArray(0);

        window->end_loop();
    } while (window->should_not_end_loop());

    glDeleteBuffers(1, &vertex_buffer);
    glDeleteProgram(program_ID);
    glDeleteVertexArrays(1, &vertex_array_ID);

    glfwTerminate();

}

void Fractal::calculate_vertices() {
    int width = window->get_width();
    int height = window->get_height();
    vertices.resize(width * height);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = y * width + x;
            vertices[index] = glm::vec2(2.0f * x / width - 1.0f, -(2.0f * y / height - 1.0f));
        }
    }
}