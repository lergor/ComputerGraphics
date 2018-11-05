#include <iostream>
#include "bunny_with_shadows.h"
#include "model/model.h"
#include "program/shaders.h"

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

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    Shaders color_program("../shaders/1.vert", "../shaders/1.frag");
    Shaders point_light_program("../shaders/point_light.vert", "../shaders/point_light.frag");

    Model bunny_model("../resources/stanford_bunny.obj", glm::scale(glm::mat4(1.0), glm::vec3(8.0f)));

    glm::vec3 lightPos(0.0f, 4.0f, 5.0f);
    Model lamp_model("../resources/cube.obj",
                     glm::scale(glm::translate(glm::mat4(0.1f), lightPos), glm::vec3(0.3f)));

    do {
        window_->process_input();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        color_program.use();
        color_program.set_vec3("object_color", 0.03f, 0.5f, 0.3f);
        color_program.set_vec3("light_color",  1.0f, 1.0f, 1.0f);

        color_program.set_vec3("light_pos", lightPos);
        color_program.set_vec3("view_pos",  window_->camera_pos());

        color_program.set_mat4("model", bunny_model.matrix());
        color_program.set_mat4("view", window_->view_matrix());
        color_program.set_mat4("projection", window_->projection_matrix());

        bunny_model.draw();

        point_light_program.use();

        point_light_program.set_mat4("model", glm::translate(lamp_model.matrix(), lightPos));
        point_light_program.set_mat4("view", window_->view_matrix());
        point_light_program.set_mat4("projection", window_->projection_matrix());

        lamp_model.draw();

        window_->end_loop();

    } while (window_->should_not_end_loop());

    color_program.delete_program();
    point_light_program.delete_program();
    bunny_model.delete_model();
    lamp_model.delete_model();
    glfwTerminate();
}