#include "bunny_with_shadows.h"

BunnyWithShadows::BunnyWithShadows(int width, int height)
        : _window_width(width), _window_height(height) {}

void BunnyWithShadows::init() {
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
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
}

void BunnyWithShadows::setup_models() {
    glm::vec3 bunny_color = glm::vec3(0.03f, 0.5f, 0.3f);
//    glm::vec3 plane_color = glm::vec3(0.2f, 0.0f, 0.4f);
    glm::vec3 plane_color = glm::vec3(1.0f);

    Material bunny_material(bunny_color, bunny_color, glm::vec3(0.5f), 32.0f);
    glm::mat4 bunny_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.17f, 0.0f));
    bunny_mat = glm::scale(bunny_mat, glm::vec3(5.0f));
    bunny_model = Model("../resources/stanford_bunny.obj", bunny_mat, bunny_material);

    Material cube_material(glm::vec3(0.05f), glm::vec3(0.8f), glm::vec3(1.0f), 32.0);
    cube_model = Model("../resources/cube.obj", glm::mat4(1.0f));

    glm::mat4 plane_mat = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));
    Material plane_material(plane_color, plane_color, glm::vec3(0.5f), 32.0f);
    plane_model = Model("../resources/plane.obj", plane_mat, plane_material);
}

void BunnyWithShadows::show() {
    init();

    lightning_program = Shaders("../shaders/lightning.vert", "../shaders/lightning.frag");
    point_light_program = Shaders("../shaders/point_light.vert", "../shaders/point_light.frag");
    shadow_depth_program = Shaders("../shaders/shadow_map_depth.vert", "../shaders/shadow_map_depth.frag");

    setup_models();
    init_shadow_map();

    shadow_depth_program.use();
    shadow_depth_program.set_int("shadowMap", 1);
    shadow_depth_program.set_bool("blinn", true);

    glm::vec3 lightPos = pointLightPositions[1];

    do {
        _window->process_input();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shadow_depth_program.use();
        shadow_depth_program.set_mat4("lightSpaceMatrix", calculate_light_matrix(lightPos));

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depth_map_FBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0f, 1.0f);
        render_scene(shadow_depth_program);
        glDisable(GL_POLYGON_OFFSET_FILL);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glViewport(0, 0, _window_width, _window_height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = _window->projection_matrix();
        glm::mat4 view = _window->view_matrix();

        lightning_program.use();
        lightning_program.set_mat4("projection", projection);
        lightning_program.set_mat4("view", view);
        lightning_program.set_mat4("lightSpaceMatrix", calculate_light_matrix(lightPos));
        lightning_program.set_vec3("viewPos", _window->camera_pos());
        lightning_program.set_vec3("lightPos", lightPos);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depth_map);

        render_scene(lightning_program);

        render_point_light(pointLightPositions[0]);
        render_point_light(pointLightPositions[1]);

        _window->end_loop();

    } while(_window->should_not_end_loop());

    finish();
}

void BunnyWithShadows::render_scene(Shaders program) {
    plane_model.import_parameters(program);
    plane_model.draw();

    bunny_model.import_parameters(program);
    bunny_model.draw();

    program.use();
    program.set_float("material.shininess", 0.25f);
    program.set_vec3("material.ambient", glm::vec3(0.0f, 0.1f, 0.06f));
    program.set_vec3("material.diffuse", glm::vec3(0.0f, 0.5f, 0.5f));
    program.set_vec3("material.specular", glm::vec3(0.5f));

    glm::mat4 model;
    model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0, -1.0f));
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
    program.set_mat4("model", model);
    cube_model.draw();
}

void BunnyWithShadows::init_shadow_map() {

    glGenFramebuffers(1, &depth_map_FBO);
    glGenTextures(1, &depth_map);
    glBindTexture(GL_TEXTURE_2D, depth_map);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glBindFramebuffer(GL_FRAMEBUFFER, depth_map_FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_map, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void BunnyWithShadows::render_point_light(glm::vec3 position) {
    lightning_program.use();
    lightning_program.set_vec3("light.ambient", glm::vec3(0.05f));
    lightning_program.set_vec3("light.diffuse", glm::vec3(1.0f));
    lightning_program.set_vec3("light.specular", glm::vec3(1.0f));
    lightning_program.set_vec3("light.position", position);

    point_light_program.use();
    point_light_program.set_mat4("projection", _window->projection_matrix());
    point_light_program.set_mat4("view", _window->view_matrix());

    glm::mat4 model = glm::mat4();
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(0.2f));
    point_light_program.set_mat4("model", model);
    cube_model.draw();
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

glm::mat4 BunnyWithShadows::calculate_light_matrix(glm::vec3 light_pos) {
    glm::mat4 light_projection, light_view;
    float near_plane = 1.0f;
    float far_plane = 7.5f;
    light_projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    light_view = glm::lookAt(light_pos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    return light_projection * light_view;
}


