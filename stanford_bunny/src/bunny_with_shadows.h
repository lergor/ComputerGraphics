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
#include "lightning/light.h"

class BunnyWithShadows {

public:

    BunnyWithShadows(int width, int height);

    void show();

private:

    void init_window();

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

    std::vector<Light> _lights;

    const unsigned int SHADOW_WIDTH = 1024;
    const unsigned int SHADOW_HEIGHT = 1024;

    void render_scene(Shaders &program);

    void finish();

    void render_point_lights();

    void draw_shadows(Light &light);
};

