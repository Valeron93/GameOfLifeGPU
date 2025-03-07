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
    if (hide_gui) {
        return;
    }
    ImGui::Begin("Settings");

    ImGui::Text("Avg. FPS: %lf", avg_fps);
    ImGui::Checkbox("Iterate", &iterate);
    ImGui::SliderInt("Iterations/sec", &iterations_per_sec, 1, 60, "%d", ImGuiSliderFlags_AlwaysClamp);

    if (ImGui::CollapsingHeader("Help")) {
        ImGui::Text("Controls:");
        ImGui::BulletText("F - Switch Fullscreen");
        ImGui::BulletText("H - Show/Hide Interface");
        ImGui::BulletText("Space - Start/Stop Iterating");
        ImGui::BulletText("Left Mouse Button - Change Cell State");
        ImGui::BulletText("Right Mouse Button - Pan Around");
    }

    if (ImGui::CollapsingHeader("Debug Info")) {
        glm::vec2 mouse;
        SDL_GetMouseState(&mouse.x, &mouse.y);
        ImGui::Text("Mouse: X: %f Y: %f", mouse.x, mouse.y);

        auto m = cam.screen_to_world(mouse);
        ImGui::Text("Mouse (world): X: %f Y: %f", m.x, m.y);
        ImGui::Text("Zoom: %f", cam.get_zoom());
    }
    ImGui::End();
}

void Application::on_event(SDL_Event* event)
{
    auto& io = ImGui::GetIO();

    switch (event->type) {
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP:
    case SDL_EVENT_MOUSE_MOTION:
    case SDL_EVENT_MOUSE_WHEEL:
        on_mouse_event(event);
        break;

    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_KEY_UP:
        on_keyboard_event(event);
        break;

    case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED:
        display_scale = SDL_GetWindowDisplayScale(window);
        break;
    }
}

void Application::on_mouse_event(SDL_Event* event)
{
    auto& io = ImGui::GetIO();
    if (io.WantCaptureMouse) {
        return;
    }
    switch (event->type) {
    case SDL_EVENT_MOUSE_MOTION:
        on_mouse_motion_event(event);
        break;

    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP:
        on_mouse_btn_event(event);
        break;

    case SDL_EVENT_MOUSE_WHEEL:
        on_mouse_wheel_event(event);
        break;
    }
}

void Application::on_mouse_motion_event(SDL_Event* event)
{
    if (!dragging) {
        return;
    }

    glm::vec2 motion = { event->motion.x, event->motion.y };
    glm::vec2 relative_motion = (motion - last_drag_point) * glm::vec2(1.0f, -1.0f);
    glm::vec2 camera_translation = relative_motion / glm::vec2(window_size);
    camera_translation.x *= float(window_size.x) / float(window_size.y);
    last_drag_point = motion;

// NOTE: on macOS with Retina displays screen panning is slower, multiplying by the
// screen scaling factors helps, but on Windows it stays consistent no matter what screen scale is.
// May be a bug in SDL.
// TODO: test this behaviour on Linux
#ifdef __APPLE__
    const float speed_coefficient = 2.0f * display_scale;
#else
    const float speed_coefficient = 2.0f;
#endif

    cam.translate(speed_coefficient * camera_translation);
}

void Application::on_keyboard_event(SDL_Event* event)
{
    auto& io = ImGui::GetIO();
    if (event->type == SDL_EVENT_KEY_DOWN) {
        if (io.WantCaptureKeyboard) {
            return;
        }
        if (event->key.key == SDLK_F) {
            fullscreen = !fullscreen;
            SDL_SetWindowFullscreen(window, fullscreen);
        } else if (event->key.key == SDLK_H) {
            hide_gui = !hide_gui;
        } else if (event->key.key == SDLK_SPACE) {
            iterate = !iterate;
        }
    }
}

void Application::on_mouse_btn_event(SDL_Event* event)
{
    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN && event->button.button == SDL_BUTTON_LEFT) {
        auto mouse = glm::vec4(event->button.x, event->button.y, 0.0f, 1.0f);

        auto mouse_position_cell = cam.screen_to_world(mouse) * glm::vec2(game_of_life.get_size());
        auto cell_state = game_of_life.get_cell((int)mouse_position_cell.x, (int)mouse_position_cell.y);
        game_of_life.set_cell((int)mouse_position_cell.x, (int)mouse_position_cell.y, !cell_state);
    } else if (event->button.button == SDL_BUTTON_RIGHT) {

        switch (event->type) {
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            dragging = true;
            SDL_GetMouseState(&last_drag_point.x, &last_drag_point.y);
            break;

        case SDL_EVENT_MOUSE_BUTTON_UP:
            dragging = false;
            break;
        }
    }
}

void Application::on_mouse_wheel_event(SDL_Event* event)
{
    float wheel = event->wheel.y;
    float scale_factor = 1.0f + (0.25f * fabsf(wheel));
    if (wheel < 0)
        scale_factor = 1.0f / scale_factor;
    cam.zoom_by({ event->wheel.mouse_x, event->wheel.mouse_y }, scale_factor, camera_min_zoom, camera_max_zoom);
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
