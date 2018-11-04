#include <iostream>
#include "bunny_with_shadows.h"
#include "model/model.h"
#include "program/shaders.h"

namespace {
    const int  WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;
}

const std::vector<GLfloat>  cube_vertices = {
        -1.0f,-1.0f,-1.0f, // triangle 1 : begin
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, // triangle 1 : end
        1.0f, 1.0f,-1.0f, // triangle 2 : begin
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f, // triangle 2 : end
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f
};


BunnyWithShadows::BunnyWithShadows(int width, int height)
        : window_width_(width), window_height_(height) {}

void BunnyWithShadows::init() {
    glewExperimental = static_cast<GLboolean>(true);
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        glfwTerminate();
        return;
    }

    window_ = Window::get_instance(window_width_, window_height_, "Stanford Bunny");

    glewExperimental = static_cast<GLboolean>(true);
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        glfwTerminate();
        return;
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
}

void BunnyWithShadows::show() {

    init();
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);

    Shaders main_program("../shaders/1.vert", "../shaders/1.frag");

    Model bunny_model("../resources/stanford_bunny.obj", glm::scale(glm::mat4(1.0), glm::vec3(8.0f)));

    do {
        window_->process_input();


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        main_program.use();
        main_program.set_mat4("model", bunny_model.matrix());
        main_program.set_mat4("view", window_->view_matrix());
        main_program.set_mat4("projection", window_->projection_matrix());

        bunny_model.draw();

        window_->end_loop();

    } while (window_->should_not_end_loop());

    main_program.delete_program();
    bunny_model.delete_model();
    glfwTerminate();
}