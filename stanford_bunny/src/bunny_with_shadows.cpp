#include "bunny_with_shadows.h"

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

    lightning_program = Shaders("../shaders/lightning.vert", "../shaders/lightning.frag");
    point_light_program = Shaders("../shaders/point_light.vert", "../shaders/point_light.frag");
    shadow_depth_program = Shaders("../shaders/shadow_depth.vert", "../shaders/shadow_depth.frag");

    glm::mat4 bunny_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));
    bunny_mat = glm::scale(bunny_mat, glm::vec3(8.0f));

    bunny_model = Model("../resources/stanford_bunny.obj", bunny_mat);


    cube_model = Model("../resources/cube.obj", glm::mat4(1.0f));

    glm::mat4 plane_mat = glm::scale(glm::translate(glm::mat4(1.0f),
                              glm::vec3(-2.5f, -4.7f, -2.5f)), glm::vec3(5.0f));
    plane_model = Model("../resources/plane.obj", plane_mat);

    init_shadow_map();

    lightning_program.use();
    lightning_program.set_bool("blinn", true);
    lightning_program.set_int("shadowMap", 1);

    do {
        window_->process_input();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightSpaceMatrix = calculate_light_matrix(lightPos);

        shadow_depth_program.use();
        shadow_depth_program.set_mat4("lightSpaceMatrix", calculate_light_matrix(lightPos));
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0f, 1.0f);
        glActiveTexture(GL_TEXTURE0);

        render_scene(shadow_depth_program);

        glDisable(GL_POLYGON_OFFSET_FILL);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glViewport(0, 0, window_->width(), window_->height());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // --------------------------------------------------------------

        glViewport(0, 0, window_->width(), window_->height());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightning_program.use();
        lightning_program.set_mat4("view", window_->view_matrix());
        lightning_program.set_mat4("projection", window_->projection_matrix());
        lightning_program.set_mat4("lightSpaceMatrix", calculate_light_matrix(lightPos));
        lightning_program.set_vec3("viewPos",  window_->camera_pos());
        lightning_program.set_vec3("lightPos", this->lightPos);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        render_scene(lightning_program);

        render_point_lights();

        window_->end_loop();

    } while (window_->should_not_end_loop());

    lightning_program.delete_program();
    point_light_program.delete_program();
    shadow_depth_program.delete_program();
    bunny_model.delete_model();
    cube_model.delete_model();
    plane_model.delete_model();
    glfwTerminate();
}

void BunnyWithShadows::render_point_lights() {
    for (auto pointLightPosition : pointLightPositions) {
        lightning_program.use();
        lightning_program.set_vec3("viewPos",  window_->camera_pos());
        lightning_program.set_vec3("lightPos", lightPos);
        lightning_program.set_vec3("light.position", pointLightPosition);
        lightning_program.set_vec3("light.ambient", glm::vec3(0.2f));
        lightning_program.set_vec3("light.diffuse",  glm::vec3(0.5f));
        lightning_program.set_vec3("light.specular", glm::vec3(1.0f));

        point_light_program.use();
        lightning_program.set_mat4("lightSpaceMatrix", calculate_light_matrix(pointLightPosition));
        point_light_program.set_mat4("view", window_->view_matrix());
        point_light_program.set_mat4("projection", window_->projection_matrix());

        glm::mat4 model = glm::translate(glm::mat4(0.1f), pointLightPosition);
        model = glm::scale(model, glm::vec3(0.3f));
        point_light_program.set_mat4("model", model);
        cube_model.draw();
    }
}

void BunnyWithShadows::init_shadow_map() {
    glGenFramebuffers(1, &depthMapFBO);
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

glm::mat4 BunnyWithShadows::calculate_light_matrix(glm::vec3 lightPos) {
    float near_plane = 1.0f, far_plane = 7.5f;
    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    return lightProjection * lightView;
}

void BunnyWithShadows::render_scene(Shaders &shader) {
    shader.use();
    shader.set_vec3("viewPos",  window_->camera_pos());
    shader.set_vec3("lightPos", lightPos);
    shader.set_mat4("view", window_->view_matrix());
    shader.set_mat4("projection", window_->projection_matrix());

    glm::vec3 diffuseColor = glm::vec3(1.0f)   * glm::vec3(0.5f); // decrease the influence
    glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
    shader.set_vec3("light.ambient", ambientColor);
    shader.set_vec3("light.diffuse", diffuseColor);
    shader.set_vec3("light.specular", glm::vec3(1.0f));
    shader.set_vec3("light.position", lightPos);

    shader.set_vec3("material.ambient", bunny_color);
    shader.set_vec3("material.diffuse", bunny_color);
    shader.set_vec3("material.specular", glm::vec3(0.5f));
    shader.set_float("material.shininess", 32.0f);
    shader.set_mat4("model", bunny_model.matrix());
    bunny_model.draw();

    shader.set_vec3("material.ambient", plane_color);
    shader.set_vec3("material.diffuse", plane_color);
    shader.set_vec3("material.specular", glm::vec3(0.5f));
    shader.set_float("material.shininess", 32.0f);
    shader.set_mat4("model", plane_model.matrix());
    plane_model.draw();
}
