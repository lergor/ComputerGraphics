#pragma once

#include <cstdio>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

};

