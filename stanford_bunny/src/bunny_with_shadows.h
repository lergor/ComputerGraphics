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

    Window *window_;
    int window_width_;
    int window_height_;

    Model bunny_model;
    Model plane_model;
    Model cube_model;

    Shaders lightning_program;
    Shaders point_light_program;
    Shaders shadow_depth_program;

    glm::vec3 bunny_color = glm::vec3(0.03f, 0.5f, 0.3f);
//    glm::vec3 plane_color = glm::vec3(0.2f, 0.0f, 0.4f);
    glm::vec3 plane_color = glm::vec3(1.0f);

    std::vector<glm::vec3> pointLightPositions = {
            glm::vec3( 2.0f, 2.0f, -4.0f),
            glm::vec3(0.0f, 4.0f, 5.0f)
    };
    glm::vec3 lightPos = glm::vec3(0.0f, 4.0f, 5.0f);
    glm::mat4 lightSpaceMatrix;

    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    unsigned int depthMapFBO;
    unsigned int depthMap;

    void render_scene(Shaders &shader);
    void render_point_lights();
    void init_shadow_map();

    glm::mat4 calculate_light_matrix(glm::vec3 lightPos);

};

