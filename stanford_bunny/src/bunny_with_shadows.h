#pragma once

#include <cstdio>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "model/model.h"
#include "program/shaders.h"
#include "program/window.h"

class BunnyWithShadows {

public:

    BunnyWithShadows(int width, int height);

    void show();

private:

    void init();

    void setup_models();

    Window *_window;
    int _window_width;
    int _window_height;

    Model bunny_model;
    Model plane_model;
    Model cube_model;

    Shaders lightning_program;
    Shaders point_light_program;
    Shaders shadow_depth_program;

    std::vector<glm::vec3> pointLightPositions = {
            glm::vec3( 2.0f, 2.0f, -4.0f),
            glm::vec3(0.0f, 4.0f, 5.0f)
    };

    glm::mat4 calculate_light_matrix(glm::vec3 light_pos);

    unsigned int depth_map_FBO;
    unsigned int depth_map;
    const unsigned int SHADOW_WIDTH = 1024;
    const unsigned int SHADOW_HEIGHT = 1024;

    void render_scene(Shaders program);

    void finish();

    void init_shadow_map();

    void render_point_light(glm::vec3 position);
};

