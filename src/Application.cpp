#include "Application.h"

#include "Shader.h"
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

Application::Application()
{
    program = shader::load_path("res/default.vert", "res/default.frag");
    int tex_uni = glGetUniformLocation(program, "tex");
    glUseProgram(program);
    glUniform1i(tex_uni, 0);

    view_projection_matrix_location = glGetUniformLocation(program, "view_projection_matrix");
    SDL_GL_SetSwapInterval(1);
}

Application::~Application()
{
    glDeleteProgram(program);
}

void Application::render()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, game_of_life.get_texture());

    glViewport(0, 0, window_size_px.x, window_size_px.y);
    glUseProgram(program);

    glUniformMatrix4fv(view_projection_matrix_location, 1, false, glm::value_ptr(cam.get_view_projection()));

    quad.draw();
}

void Application::imgui()
{
    ImGui::Begin("Debug Menu");

    ImGui::Text("Avg. FPS: %lf", avg_fps);
    ImGui::Checkbox("Iterate", &iterate);
    ImGui::SliderInt("Iterations/sec", &iterations_per_sec, 1, 60, "%d", ImGuiSliderFlags_AlwaysClamp);

    ImGui::End();
}

void Application::on_event(SDL_Event* event)
{
    auto& io = ImGui::GetIO();
    if (io.WantCaptureMouse) {
        return;
    }
    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN && event->button.button == SDL_BUTTON_LEFT) {
        auto mouse = glm::vec4(event->button.x, event->button.y, 0.0f, 1.0f);

        auto mouse_position_cell = cam.screen_to_world(mouse) * glm::vec2(game_of_life.get_size());
        auto cell_state = game_of_life.get_cell((int)mouse_position_cell.x, (int)mouse_position_cell.y);
        game_of_life.set_cell((int)mouse_position_cell.x, (int)mouse_position_cell.y, !cell_state);
    } else if ((event->type == SDL_EVENT_MOUSE_BUTTON_DOWN || event->type == SDL_EVENT_MOUSE_BUTTON_UP) && event->button.button == SDL_BUTTON_RIGHT) {

        if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            dragging = true;
        } else {
            dragging = false;
        }

    } else if (dragging && event->type == SDL_EVENT_MOUSE_MOTION) {
        glm::vec2 relative_motion = { event->motion.xrel, -event->motion.yrel };

        glm::vec2 camera_translation = relative_motion / glm::vec2(window_size_px);
        const float speed_coefficient = 2.0f;

        cam.translate(speed_coefficient * camera_translation);
    } else if (event->type == SDL_EVENT_MOUSE_WHEEL) {
        float wheel = event->wheel.y;
        float scale_factor = 1.0f + (0.25f * fabsf(wheel));
        if (wheel < 0)
            scale_factor = 1.0f / scale_factor;

        glm::vec2 mouse_pos;
        SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
        cam.zoom_by(mouse_pos, scale_factor, camera_min_zoom, camera_max_zoom);
    }
}

void Application::update()
{
    auto fps = 1.0 / delta_time;
    avg_fps = (avg_fps + fps) * 0.5;

    SDL_GetWindowSize(window, &window_size.x, &window_size.y);
    SDL_GetWindowSizeInPixels(window, &window_size_px.x, &window_size_px.y);
    cam.set_size(window_size);

    auto keyboard = SDL_GetKeyboardState(nullptr);

    auto& io = ImGui::GetIO();

    iteration_timer += delta_time;

    if (iterate && iteration_timer > 1.0f / float(iterations_per_sec)) {
        game_of_life.iterate(quad);
        iteration_timer = 0;
    }
}
