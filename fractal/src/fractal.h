#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "load_shaders.h"
#include "window.h"

class Fractal {

public:

    Fractal(int width, int height, int max_iter, float max_radius);

    void show();

private:

    void calculate_vertices();

    void init();

    Window *window;
    int window_width;
    int window_height;
    int max_iter;
    float max_radius;
    std::vector<glm::vec2> vertices;

};
