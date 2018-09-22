#include "window.h"

namespace {

    void mouse_pos_callback(GLFWwindow *_window, double x, double y) {
        Window *window = Window::get_instance();
        glm::ivec2 cur_mouse_pos(x, y);
        glm::vec2 prev_mouse_pos = window->get_mouse_pos();
        if (glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
            auto normalized_cur = window->normalize_coords(cur_mouse_pos.x, cur_mouse_pos.y);
            auto normalized_prev = window->normalize_coords(prev_mouse_pos.x, prev_mouse_pos.y);
            glm::vec2 delta_pos = normalized_cur - normalized_prev;
            window->change_shift(-delta_pos);
        }
        window->set_mouse_pos(cur_mouse_pos);
    }

    void mouse_wheel_callback(GLFWwindow *, double, double y_offset) {
        Window *window = Window::get_instance();
        glm::vec2 mouse_pos = window->get_mouse_pos();
        auto normalized_pos = window->normalize_coords(mouse_pos.x, mouse_pos.y);
        auto normalized_prev = normalized_pos;
        if (y_offset < 0) {
            window->increase_zoom();
        } else {
            window->decrease_zoom();
        }
        auto normalized_after = window->normalize_coords(mouse_pos.x, mouse_pos.y);
        auto shiftDelta = normalized_prev - normalized_after;
        window->change_shift(shiftDelta);
    }

}

Window *Window::instance = nullptr;

Window::Window(int width, int height)
        : width(width),
          height(height)
{
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Mandelbrot", nullptr, nullptr);

    if (window == nullptr) {
        fprintf(stderr, "Unable to create GLFW window\n");
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetCursorPosCallback(window, static_cast<GLFWcursorposfun>(mouse_pos_callback));
    glfwSetScrollCallback(window, static_cast<GLFWscrollfun>(mouse_wheel_callback));
}

Window *Window::get_instance() {
    if (!instance)
        assert(0);
    return instance;
}

Window *Window::get_instance(int width, int height) {
    if (!instance)
        instance = new Window(width, height);
    return instance;
}

int Window::get_width() {
    return width;
}

int Window::get_height() {
    return height;
}

glm::vec2 Window::get_scale() {
    glm::vec2 vecScale(zoom, zoom);
    if (height > width) {
        vecScale.y *= float(height) / width;
    } else {
        vecScale.x *= float(width) / height;
    }
    return vecScale;
}

glm::vec2 Window::get_shift() {
    return shift;
}

glm::vec2 Window::get_mouse_pos() {
    return mouse_coordinates;
}

void Window::change_shift(glm::vec2 delta_shift) {
    shift += delta_shift;
}

void Window::increase_zoom() {
    zoom *= zoom_multiplier;
}

void Window::decrease_zoom() {
    zoom /= zoom_multiplier;
}

glm::vec2 Window::normalize_coords(int x, int y) {
    return get_scale() * glm::vec2(2.0f * x / width - 1.0f, -(2.0f * y / height - 1.0f));
}

bool Window::should_not_end_loop() {
    return glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0;
}

void Window::end_loop() {
    glfwSwapBuffers(window);
    glfwPollEvents();
}

Window::~Window() {
    delete instance;
}

void Window::set_mouse_pos(glm::vec2 new_pos) {
    mouse_coordinates = new_pos;
}

