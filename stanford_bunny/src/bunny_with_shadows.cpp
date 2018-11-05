#include <iostream>
#include "bunny_with_shadows.h"
#include "model/model.h"
#include "program/shaders.h"

float fieldVertices[] = {
        // positions
        1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // top right
        1.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f, // bottom right
        -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom left
        -1.0f,  0.0f, -1.0f, 0.0f, 1.0f, 0.0f // top left
};
unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
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

    Shaders lightning_program("../shaders/lightning.vert", "../shaders/lightning.frag");
    Shaders point_light_program("../shaders/point_light.vert", "../shaders/point_light.frag");

    glm::mat4 bunny_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));
    bunny_mat = glm::scale(bunny_mat, glm::vec3(8.0f));

    Model bunny_model("../resources/stanford_bunny.obj", bunny_mat);

    glm::vec3 lightPos(0.0f, 4.0f, 5.0f);
    glm::vec3 bunny_color(0.03f, 0.5f, 0.3f);
    glm::vec3 plane_color(0.2f, 0.0f, 0.4f);

    Model cube("../resources/cube.obj", glm::mat4(1.0f));

    glm::mat4 plane_mat = glm::scale(glm::translate(glm::mat4(1.0f),
                              glm::vec3(-2.5f, -4.7f, -2.5f)), glm::vec3(5.0f));

    Model plane("../resources/plane.obj", plane_mat);

    glm::vec3 pointLightPositions[] = {
            glm::vec3( 2.0f, 2.0f, -4.0f),
            glm::vec3(0.0f, 4.0f, 5.0f)
    };

    // material properties
    lightning_program.use();
    lightning_program.set_bool("blinn", true);
    lightning_program.set_vec3("material.ambient", bunny_color);
    lightning_program.set_vec3("material.diffuse", bunny_color);
    lightning_program.set_vec3("material.specular", glm::vec3(0.5f));
    lightning_program.set_float("material.shininess", 32.0f);

    do {
        window_->process_input();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightning_program.use();
//        color_program.set_vec3("object_color", bunny_color);
//        color_program.set_vec3("light_color",  glm::vec3(1.0f));

        lightning_program.set_vec3("viewPos",  window_->camera_pos());

        lightning_program.set_vec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        lightning_program.set_vec3("dirLight.ambient", glm::vec3( 0.05f));
        lightning_program.set_vec3("dirLight.diffuse",  glm::vec3(0.4f));
        lightning_program.set_vec3("dirLight.specular", glm::vec3(0.5f));

        lightning_program.set_vec3("pointLights[0].position", pointLightPositions[0]);
        lightning_program.set_vec3("pointLights[0].ambient", glm::vec3(0.05f));
        lightning_program.set_vec3("pointLights[0].diffuse", glm::vec3(0.8f));
        lightning_program.set_vec3("pointLights[0].specular", glm::vec3(1.0f));
        lightning_program.set_float("pointLights[0].constant", 1.0f);
        lightning_program.set_float("pointLights[0].linear", 0.09);
        lightning_program.set_float("pointLights[0].quadratic", 0.032);

        lightning_program.set_vec3("pointLights[1].position", pointLightPositions[1]);
        lightning_program.set_vec3("pointLights[1].ambient", glm::vec3(0.05f));
        lightning_program.set_vec3("pointLights[1].diffuse", glm::vec3(0.8f));
        lightning_program.set_vec3("pointLights[1].specular", glm::vec3(1.0f));
        lightning_program.set_float("pointLights[1].constant", 1.0f);
        lightning_program.set_float("pointLights[1].linear", 0.09);
        lightning_program.set_float("pointLights[1].quadratic", 0.032);

        lightning_program.set_vec3("spotLight.position", window_->camera_pos());
        lightning_program.set_vec3("spotLight.direction", window_->camera_dir());
        lightning_program.set_vec3("spotLight.ambient", glm::vec3(0.0f));
        lightning_program.set_vec3("spotLight.diffuse", glm::vec3(1.0f));
        lightning_program.set_vec3("spotLight.specular", glm::vec3(1.0f));
        lightning_program.set_float("spotLight.constant", 1.0f);
        lightning_program.set_float("spotLight.linear", 0.09);
        lightning_program.set_float("spotLight.quadratic", 0.032);
        lightning_program.set_float("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        lightning_program.set_float("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        lightning_program.set_mat4("model", bunny_model.matrix());
        lightning_program.set_mat4("view", window_->view_matrix());
        lightning_program.set_mat4("projection", window_->projection_matrix());

        lightning_program.set_vec3("material.ambient", bunny_color);
        lightning_program.set_vec3("material.diffuse", bunny_color);
        bunny_model.draw();

        lightning_program.set_vec3("material.ambient", plane_color);
        lightning_program.set_vec3("material.diffuse", plane_color);
        lightning_program.set_mat4("model", plane.matrix());
        plane.draw();

        point_light_program.use();
        point_light_program.set_mat4("view", window_->view_matrix());
        point_light_program.set_mat4("projection", window_->projection_matrix());
        for (auto pointLightPosition : pointLightPositions) {
            glm::mat4 model = glm::translate(glm::mat4(0.1f), pointLightPosition);
            model = glm::scale(model, glm::vec3(0.3f));
            point_light_program.set_mat4("model", model);
            cube.draw();
        }

        window_->end_loop();

    } while (window_->should_not_end_loop());

    lightning_program.delete_program();
    point_light_program.delete_program();
    bunny_model.delete_model();
    cube.delete_model();
    plane.delete_model();
    glfwTerminate();
}