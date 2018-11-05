#pragma once

#include <glm/vec2.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <string>
#include "camera.h"

class Window {

public:

    static Window *get_instance();

    static Window *get_instance(int width, int height, std::string const &name);

    ~Window();

    bool should_not_end_loop();

    void end_loop();

    glm::mat4 projection_matrix();

    glm::mat4 view_matrix();

    void process_input();

    void frame_buffer_callback();

    void mouse_callback(double x_pos, double y_pos);

    void scroll_callback( double y_offset);

    glm::vec3 camera_pos();

    glm::vec3 camera_dir();

    int width();

    int height();

private:

    Window(int width, int height, std::string const &name);

    static Window *_instance;
    GLFWwindow *_window;
    int _width;
    int _height;

    Camera _camera;
    float _last_x;
    float _last_y;
    bool _first_mouse = true;
    float _delta_time = 0.0f;
    float _last_time = 0.0f;
    float _initial_FoV = 45.0f;
};
