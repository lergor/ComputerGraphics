#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <GL/glew.h>

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  3.0f;
const float SENSITIVITY =  0.2f;
const float ZOOM        =  45.0f;

class Camera {

public:

    glm::vec3 _position;
    glm::vec3 _direction;
    glm::vec3 _right;
    glm::vec3 _up;
    glm::vec3 _world_up;
    float _yaw;
    float _pitch;
    float _movement_speed;
    float _mouse_sensitivity;
    float _zoom;

    explicit Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
            : _direction(glm::vec3(0.0f, 0.0f, -1.0f))
            , _movement_speed(SPEED)
            , _mouse_sensitivity(SENSITIVITY)
            , _zoom(ZOOM)
            , _position(position)
            , _world_up(up)
            , _yaw(yaw)
            , _pitch(pitch)
    {
        update_vectors();
    }

    explicit Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
            : _direction(glm::vec3(0.0f, 0.0f, -1.0f))
            , _movement_speed(SPEED)
            , _mouse_sensitivity(SENSITIVITY)
            , _zoom(ZOOM)
            , _position(glm::vec3(posX, posY, posZ))
            , _world_up(glm::vec3(upX, upY, upZ))
            , _pitch(pitch)
    {
        update_vectors();
    }

    void process_mouse_movement(float x_offset, float y_offset, GLboolean constrain_ritch = true) {
        _yaw   += x_offset * _mouse_sensitivity;
        _pitch += y_offset * _mouse_sensitivity;

        float pitch_limit = 89.0f;
        if (constrain_ritch) {
            _pitch = std::min(_pitch, pitch_limit);
            _pitch = std::max(_pitch, -pitch_limit);
        }
        update_vectors();
    }

    void update_vectors() {
        glm::vec3 front = {
            static_cast<float>(cos(glm::radians(_yaw)) * cos(glm::radians(_pitch))),
            static_cast<float>(sin(glm::radians(_pitch))),
            static_cast<float>(sin(glm::radians(_yaw)) * cos(glm::radians(_pitch))),
        };

        _direction = glm::normalize(front);

        _right = glm::normalize(glm::cross(_direction, _world_up));
        _up    = glm::normalize(glm::cross(_right, _direction));
    }

    void process_mouse_scroll(float y_offset) {
        float zoom_min = 1.0f;
        float zoom_max = 45.0f;

        if (_zoom >= zoom_min && _zoom <= zoom_max)
            _zoom -= y_offset;
        _zoom = std::max(_zoom, zoom_min);
        _zoom = std::min(_zoom, zoom_max);

    }

    void process_keyboard_input(CameraMovement direction, float deltaTime) {
        float velocity = _movement_speed * deltaTime;
        switch (direction) {
            case FORWARD: _position += _direction * velocity; break;
            case BACKWARD: _position -= _direction * velocity; break;
            case LEFT: _position -= _right * velocity; break;
            case RIGHT: _position += _right * velocity; break;
            default: break;
        }
    }

    glm::mat4 view_matrix() {
        return glm::lookAt(_position, _position + _direction, _up);
    }

};
