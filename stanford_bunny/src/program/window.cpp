#include <GL/glew.h>
#include "window.h"

namespace {

    void mouse_pos_callback(GLFWwindow *, double x, double y) {
        Window::get_instance()->mouse_callback(x, y);
    }

    void mouse_wheel_callback(GLFWwindow *, double, double y_offset) {
        Window::get_instance()->scroll_callback(y_offset);
    }

    void frame_buffer_size_callback(GLFWwindow *window, int width, int height) {
        Window::get_instance()->frame_buffer_callback();
    }

}
Window *Window::_instance = nullptr;

Window::Window(int width, int height, std::string const &name)
        : _width(width), _height(height), _camera(Camera(glm::vec3(0.0f, 1.5f, 5.0f))), _last_x(_width / 2),
          _last_y(_height / 2) {
    glfwWindowHint(GLFW_SAMPLES, 4); // nope
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    _window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

    if (_window == nullptr) {
        fprintf(stderr, ("Unable to create " + name + " window\n").data());
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(_window);

    glfwSetCursorPosCallback(_window, static_cast<GLFWcursorposfun>(mouse_pos_callback));
    glfwSetScrollCallback(_window, static_cast<GLFWscrollfun>(mouse_wheel_callback));
    glfwSetFramebufferSizeCallback(_window, frame_buffer_size_callback);


    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(_window, GLFW_STICKY_KEYS, GL_TRUE);
}

Window *Window::get_instance() {
    if (!_instance)
        assert(0);
    return _instance;
}

Window *Window::get_instance(int width, int height, std::string const &name) {
    if (!_instance)
        _instance = new Window(width, height, name);
    return _instance;
}

bool Window::should_not_end_loop() {
    return glfwGetKey(_window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(_window) == 0;
}

void Window::end_loop() {
    glfwSwapBuffers(_window);
    glfwPollEvents();
}

Window::~Window() {
    delete _instance;
}

glm::mat4 Window::view_matrix() {
    return _camera.view_matrix();
}

glm::mat4 Window::projection_matrix() {
    return glm::perspective(glm::radians(_camera._zoom), (float) _width / (float) _height, 0.1f, 100.0f);
}

void Window::process_input() {
    auto current_time = static_cast<float>(glfwGetTime());
    _delta_time = current_time - _last_time;
    _last_time = current_time;

    if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(_window, true);

    if (glfwGetKey(_window, GLFW_KEY_UP) == GLFW_PRESS)
        _camera.process_keyboard_input(FORWARD, _delta_time);
    if (glfwGetKey(_window, GLFW_KEY_DOWN) == GLFW_PRESS)
        _camera.process_keyboard_input(BACKWARD, _delta_time);
    if (glfwGetKey(_window, GLFW_KEY_LEFT) == GLFW_PRESS)
        _camera.process_keyboard_input(LEFT, _delta_time);
    if (glfwGetKey(_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        _camera.process_keyboard_input(RIGHT, _delta_time);
}

void Window::mouse_callback(double x_pos, double y_pos) {
    if (_first_mouse) {
        _last_x = static_cast<float>(x_pos);
        _last_y = static_cast<float>(y_pos);
        _first_mouse = false;
    }

    auto x_offset = static_cast<float>(x_pos - _last_x);
    auto y_offset = static_cast<float>(_last_y - y_pos);
    _last_x = static_cast<float>(x_pos);
    _last_y = static_cast<float>(y_pos);

    _camera.process_mouse_movement(x_offset, y_offset);
}

void Window::scroll_callback(double y_offset) {
    _camera.process_mouse_scroll(static_cast<float>(y_offset));
}

void Window::frame_buffer_callback() {
    glViewport(0, 0, _width, _height);
}

glm::vec3 Window::camera_pos()  {
    return _camera._position;
}

int Window::width() {
    return _width;
}

int Window::height() {
    return _height;
}

