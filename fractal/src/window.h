#pragma once

#include <glm/vec2.hpp>
#include <GLFW/glfw3.h>
#include <iostream>

class Window {

public:

    static Window *get_instance();

    static Window *get_instance(int width, int height);

    ~Window();

    int get_width();

    int get_height();

    glm::vec2 get_scale();

    glm::vec2 get_shift();

    glm::vec2 get_mouse_pos();

    void set_mouse_pos(glm::vec2 new_pos);

    void change_shift(glm::vec2 delta_shift);

    void increase_zoom();

    void decrease_zoom();

    glm::vec2 normalize_coords(int x, int y);

    bool should_not_end_loop();

    void end_loop();

private:

    Window(int width, int height);

    static Window *instance;
    int width;
    int height;
    float zoom = 1.0f;
    float zoom_multiplier = 1.1f;
    GLFWwindow *window;
    glm::vec2 mouse_coordinates;
    glm::vec2 shift;
};
