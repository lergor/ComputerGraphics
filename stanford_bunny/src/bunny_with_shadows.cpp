#include "bunny_with_shadows.h"

BunnyWithShadows::BunnyWithShadows(int width, int height)
        : _window_width(width), _window_height(height) {}

void BunnyWithShadows::init_window() {
    glewExperimental = static_cast<GLboolean>(true);
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        glfwTerminate();
        return;
    }

    _window = Window::get_instance(_window_width, _window_height, "Stanford Bunny");

    glewExperimental = static_cast<GLboolean>(true);
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        glfwTerminate();
        return;
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearColor(b_clr[0], b_clr[1], b_clr[2], b_clr[3]);
}

void BunnyWithShadows::setup_models() {
    glm::vec3 bunny_color = glm::vec3(0.03f, 0.5f, 0.3f);
    glm::vec3 plane_color = glm::vec3(0.5f);

    Material bunny_material(bunny_color, bunny_color, glm::vec3(0.5f), 32.0f);
    glm::mat4 bunny_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.17f, 0.0f));
    bunny_mat = glm::scale(bunny_mat, glm::vec3(5.0f));
    bunny_model = Model("../resources/stanford_bunny.obj", bunny_mat, bunny_material, true);

    glm::mat4 cube_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.5f));
    cube_mat = glm::scale(cube_mat, glm::vec3(0.6f));
    Material cube_material(glm::vec3(0.05f), glm::vec3(0.8f), glm::vec3(1.0f), 32.0);
    cube_model = Model("../resources/cube.obj", cube_mat);

    glm::mat4 plane_mat = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));
    Material plane_material(plane_color, plane_color, glm::vec3(0.5f), 32.0f);
    plane_model = Model("../resources/plane.obj", plane_mat, plane_material);
}

void BunnyWithShadows::show() {
    init_window();

    lightning_program = Shaders("../shaders/lightning.vert", "../shaders/lightning.frag");
    point_light_program = Shaders("../shaders/point_light.vert", "../shaders/point_light.frag");
    shadow_depth_program = Shaders("../shaders/shadow_map_depth.vert", "../shaders/shadow_map_depth.frag");

    setup_models();

    _lights.emplace_back(glm::vec3(0.0f, 4.0f, 5.0f), 0, GL_TEXTURE0);
//    _lights.emplace_back(glm::vec3(2.0f, 2.0f, -2.0f), 1, GL_TEXTURE1);

    lightning_program.use();
    lightning_program.set_bool("blinn", true);
    for (auto light : _lights) {
        light.import_parameters(lightning_program);
    }

    do {
        _window->process_input();

        glClearColor(b_clr[0], b_clr[1], b_clr[2], b_clr[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for(auto light : _lights) {
            draw_shadows(light);
        }

        glViewport(0, 0, _window_width, _window_height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightning_program.use();
        lightning_program.set_mat4("projection", _window->projection_matrix());
        lightning_program.set_mat4("view", _window->view_matrix());
        lightning_program.set_vec3("viewPos", _window->camera_pos());

        for (auto light : _lights) {
            glActiveTexture(light.texture_unit());
            glBindTexture(GL_TEXTURE_2D, light.depth_map());
        }
        render_scene(lightning_program);
        render_point_lights();
        _window->end_loop();

    } while (_window->should_not_end_loop());

    finish();
}

void BunnyWithShadows::render_scene(Shaders &program) {
    plane_model.import_parameters(program);
    plane_model.draw();

    bunny_model.import_parameters(program);
    bunny_model.draw();

//    cube_model.import_parameters(program);
//    cube_model.draw();
}

void BunnyWithShadows::render_point_lights() {
    lightning_program.use();
    lightning_program.set_mat4("projection", _window->projection_matrix());
    lightning_program.set_mat4("view", _window->view_matrix());
    lightning_program.set_vec3("viewPos", _window->camera_pos());

    point_light_program.use();
    point_light_program.set_mat4("projection", _window->projection_matrix());
    point_light_program.set_mat4("view", _window->view_matrix());

    for (auto light : _lights) {
        glm::mat4 model;
        model = glm::translate(glm::mat4(), light.position());
        model = glm::scale(model, glm::vec3(0.2f));
        point_light_program.set_mat4("model", model);
        cube_model.draw();
    }
}

void BunnyWithShadows::draw_shadows(Light &light) {

    glBindFramebuffer(GL_FRAMEBUFFER, light.depth_map_FBO());
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0f, 1.0f);

    shadow_depth_program.use();
    shadow_depth_program.set_mat4("lightSpaceMatrix", light.light_VP_matrix());
    render_scene(shadow_depth_program);

    glDisable(GL_POLYGON_OFFSET_FILL);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void BunnyWithShadows::finish() {
    bunny_model.clear();
    plane_model.clear();
    cube_model.clear();
    point_light_program.finish();
    shadow_depth_program.finish();
    lightning_program.finish();
    glfwTerminate();
}

