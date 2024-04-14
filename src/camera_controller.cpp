#include "camera_controller.h"
#include "camera.h"
#include <input/input_events.h>

CameraController::CameraController()
{
    _mouse_event_handle = Input::mouse_event.bind(EVENT_CB(CameraController::on_mouse_event_received);
}

CameraController::~CameraController()
{
    Input::mouse_event.unbind(_mouse_event_handle);
}

void CameraController::set_camera(Camera* camera)
{
    if (_camera == camera)
        return;
    _camera = camera;
}

void CameraController::on_mouse_event_received(MouseEvent mouse_event)
{

}

void CameraController::on_mouse_position(float x, float y)
{
    if (!_grabbing)
        return;

    glm::vec2 offset = _start_point - glm::vec2(x, y);
    _start_point = glm::vec2(x, y);

    const float turn_rate = 0.001f;
    glm::vec3 euler = _camera->get_euler();
    euler.x += offset.y * turn_rate;
    euler.y += offset.x * turn_rate;
    _camera->set_euler(euler);
}

void CameraController::on_mouse_button(int button, int action)
{
    glm::vec2 mouse_pos = Input::get()->get_mouse_position();
    switch (action)
    {
        case 0:
            // Button up
            if (button == 1)
            {
                end();
            }
            break;
        case 1:
            // Button down
            if (button == 1)
            {
                begin(mouse_pos.x, mouse_pos.y);
            }
            break;
        default:
            break;
    }
}

void CameraController::on_mouse_scroll(float offset)
{
    glm::vec3 camera_pos = _camera->get_translation();
    glm::mat4 camera_transform = _camera->get_transform();
    glm::vec3 camera_front = glm::vec3(camera_transform[2][0], camera_transform[2][1], camera_transform[2][2]);

    const float speed = 0.2f;
    camera_pos -= camera_front * offset * speed;
    _camera->set_translation(camera_pos);
}

