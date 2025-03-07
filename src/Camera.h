#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {

    glm::vec2 target = glm::vec2(0);
    glm::vec2 screen_size = glm::vec2(0);
    float zoom = 1.0f;
    bool dirty = true;

    glm::mat4 view_projection_matrix;

public:
    const glm::mat4& get_view_projection();
    glm::vec2 screen_to_world(glm::vec2 point);

    void zoom_by(glm::vec2 mouse_point, float scale, float zoom_min, float zoom_max);
    void translate(glm::vec2 translation);
    void set_size(glm::vec2 screen_size);

    float get_zoom() const;

private:
    void clamp_target_position();
};