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
    glm::vec3 bunny_color(0.03f, 0.5f, 0.3f);
    Model lamp_model("../resources/cube.obj", glm::mat4(1.0f));

    glm::vec3 pointLightPositions[] = {
            glm::vec3( 2.0f, 2.0f, -4.0f),
            glm::vec3(0.0f, 4.0f, 5.0f)
    };

    // material properties
    color_program.use();
    color_program.set_vec3("material.ambient", bunny_color);
    color_program.set_vec3("material.diffuse", bunny_color);
    color_program.set_vec3("material.specular", glm::vec3(0.5f));
    color_program.set_float("material.shininess", 32.0f);

    do {
        window_->process_input();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        color_program.use();
//        color_program.set_vec3("object_color", bunny_color);
//        color_program.set_vec3("light_color",  glm::vec3(1.0f));

        color_program.set_vec3("viewPos",  window_->camera_pos());

        color_program.set_vec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        color_program.set_vec3("dirLight.ambient", glm::vec3( 0.05f));
        color_program.set_vec3("dirLight.diffuse",  glm::vec3(0.4f));
        color_program.set_vec3("dirLight.specular", glm::vec3(0.5f));

        color_program.set_vec3("pointLights[0].position", pointLightPositions[0]);
        color_program.set_vec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        color_program.set_vec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        color_program.set_vec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        color_program.set_float("pointLights[0].constant", 1.0f);
        color_program.set_float("pointLights[0].linear", 0.09);
        color_program.set_float("pointLights[0].quadratic", 0.032);

        color_program.set_vec3("pointLights[1].position", pointLightPositions[1]);
        color_program.set_vec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        color_program.set_vec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        color_program.set_vec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        color_program.set_float("pointLights[1].constant", 1.0f);
        color_program.set_float("pointLights[1].linear", 0.09);
        color_program.set_float("pointLights[1].quadratic", 0.032);

        color_program.set_vec3("spotLight.position", window_->camera_pos());
        color_program.set_vec3("spotLight.direction", window_->camera_dir());
        color_program.set_vec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        color_program.set_vec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        color_program.set_vec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        color_program.set_float("spotLight.constant", 1.0f);
        color_program.set_float("spotLight.linear", 0.09);
        color_program.set_float("spotLight.quadratic", 0.032);
        color_program.set_float("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        color_program.set_float("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        color_program.set_mat4("model", bunny_model.matrix());
        color_program.set_mat4("view", window_->view_matrix());
        color_program.set_mat4("projection", window_->projection_matrix());

        bunny_model.draw();

        point_light_program.use();
        point_light_program.set_mat4("view", window_->view_matrix());
        point_light_program.set_mat4("projection", window_->projection_matrix());

        for (unsigned int i = 0; i < 2; i++) {
            glm::mat4 model = glm::translate(glm::mat4(0.1f), pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.3f)); // Make it a smaller cube
            point_light_program.set_mat4("model", model);
            lamp_model.draw();
        }

        window_->end_loop();

    } while (window_->should_not_end_loop());

    color_program.delete_program();
    point_light_program.delete_program();
    bunny_model.delete_model();
    lamp_model.delete_model();
    glfwTerminate();
}