#include "Camera.h"

const glm::mat4& Camera::get_view_projection()
{
    if (dirty) {
        auto identity = glm::mat4(1.0f);
        auto view_matrix = glm::translate(identity, glm::vec3(target, 0.0f));

        auto aspect_ratio = screen_size.x / screen_size.y;
        float half_width = (1.0f / zoom) * aspect_ratio;
        float half_height = (1.0f / zoom);

        auto projection_matrix = glm::ortho(-half_width, half_width, -half_height, half_height, -1.0f, 1.0f);
        dirty = false;

        view_projection_matrix = projection_matrix * view_matrix;
    }
    return view_projection_matrix;
}

glm::vec2 Camera::screen_to_world(glm::vec2 point)
{
    float x = (2.0f * point.x) / screen_size.x - 1.0f;
    float y = 1.0f - (2.0f * point.y) / screen_size.y;

    auto world_ndc = glm::inverse(get_view_projection()) * glm::vec4(x, y, 0.0f, 1.0f);
    return (world_ndc + glm::vec4(1.0f)) / glm::vec4(2.0f);
}

void Camera::zoom_by(glm::vec2 mouse_point, float scale, float zoom_min, float zoom_max)
{
    dirty = true;
    glm::vec2 world_mouse = screen_to_world(mouse_point);
    zoom = glm::clamp(zoom * scale, zoom_min, zoom_max);
    glm::vec2 world_mouse_new = screen_to_world(mouse_point);

    glm::vec2 zoom_translation = world_mouse - world_mouse_new;
    target -= 2.0f * zoom_translation;
}

void Camera::translate(glm::vec2 translation)
{
    dirty = true;
    target += translation / zoom;
}

void Camera::set_size(glm::vec2 screen_size)
{
    dirty = true;
    this->screen_size = screen_size;
}

float Camera::get_zoom() const
{
    return zoom;
}
