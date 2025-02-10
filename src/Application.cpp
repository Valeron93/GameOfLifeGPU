#include "Application.h"

#include "Shader.h"
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

struct Vertex {
    glm::vec2 position;
    glm::vec2 uv;
};

glm::mat4 orthographic_view(float width, float height, float zoom)
{

    auto aspect_ratio = width / height;
    float half_width = (1.0f / zoom) * aspect_ratio;
    float half_height = (1.0f / zoom);

    return glm::ortho(-half_width, half_width, -half_height, half_height, -1.0f, 1.0f);
}

glm::vec4 window_to_ndc(glm::vec2 position, glm::vec2 window_size)
{
    float x = (2.0f * position.x) / window_size.x - 1.0f;
    float y = 1.0f - (2.0f * position.y) / window_size.y;
    return glm::vec4(x, y, 0.0f, 1.0f);
}

glm::vec2 ndc_to_world(glm::vec4 ndc, const glm::mat4& projection, const glm::mat4& view)
{
    auto position = ((glm::inverse(projection * view) * ndc) + glm::vec4(1.0f)) / glm::vec4(2.0f);
    return { position.x, position.y };
}

Application::Application()
{
    glGenBuffers(1, &vertex_buffer);
    glGenVertexArrays(1, &vertex_array);

    Vertex vertices[] = {
        { { -1.0f, -1.0f }, { 0, 0 } },
        { { 1.0f, -1.0f }, { 1.0f, 0.0f } },
        { { -1.0f, 1.0f }, { 0.0f, 1.0f } },

        { { 1.0f, 1.0f }, { 1.0f, 1.0f } },
        { { -1.0f, 1.0f }, { 0.0f, 1.0f } },
        { { 1.0f, -1.0f }, { 1.0f, 0.0f } },
    };

    glBindVertexArray(vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glEnableVertexAttribArray(1);

    program = shader::load_path("res/default");
    int tex_uni = glGetUniformLocation(program, "tex");
    glUseProgram(program);
    glUniform1i(tex_uni, 0);

    view_matrix_location = glGetUniformLocation(program, "view_matrix");
    projection_matrix_location = glGetUniformLocation(program, "projection_matrix");

    SDL_GL_SetSwapInterval(1);
}

Application::~Application()
{
    glDeleteProgram(program);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteVertexArrays(1, &vertex_array);
}

void Application::render()
{

    if (iterate) {
        game_of_life.iterate();
    }

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, game_of_life.get_texture());

    glViewport(0, 0, window_size_px.x, window_size_px.y);
    glUseProgram(program);

    glUniformMatrix4fv(view_matrix_location, 1, false, glm::value_ptr(view_matrix));
    glUniformMatrix4fv(projection_matrix_location, 1, false, glm::value_ptr(projection_matrix));

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Application::imgui()
{
    ImGui::Begin("Debug Menu");

    ImGui::Text("OpenGL Version: %s", glGetString(GL_VERSION));
    ImGui::Text("OpenGL Renderer: %s", glGetString(GL_RENDERER));

    ImGui::Text("Avg. FPS: %lf", avg_fps);

    ImGui::Checkbox("Iterate", &iterate);
    ImGui::SliderFloat2("Camera position", glm::value_ptr(camera_position), -1.0f, 1.0f);
    ImGui::SliderFloat("Zoom", &camera_zoom, 0.125f, 64.0f);
    ImGui::End();
}

void Application::on_event(SDL_Event* event)
{
    auto& io = ImGui::GetIO();
    if (io.WantCaptureMouse) {
        return;
    }
    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {

        auto mouse = glm::vec4(event->button.x, event->button.y, 0.0f, 1.0f);
        auto ndc = window_to_ndc(mouse, window_size_px);
        auto mouse_position_cell = ndc_to_world(ndc, projection_matrix, view_matrix) * glm::vec2(game_of_life.get_size());
        auto cell_state = game_of_life.get_cell((int)mouse_position_cell.x, (int)mouse_position_cell.y);
        game_of_life.set_cell((int)mouse_position_cell.x, (int)mouse_position_cell.y, !cell_state);
    } else if (event->type == SDL_EVENT_MOUSE_WHEEL) {
        float wheel = event->wheel.y;
        float scale_factor = 1.0f + (0.25f * fabsf(wheel));
        if (wheel < 0)
            scale_factor = 1.0f / scale_factor;

        camera_zoom = glm::clamp(camera_zoom * scale_factor, 0.5f, 64.0f);
    }
}

void Application::update()
{
    auto fps = 1.0 / delta_time;
    avg_fps = (avg_fps + fps) * 0.5;

    SDL_GetWindowSize(window, &window_size.x, &window_size.y);
    SDL_GetWindowSizeInPixels(window, &window_size_px.x, &window_size_px.y);

    auto keyboard = SDL_GetKeyboardState(nullptr);
    auto factor = delta_time * 2.0f * 1.0f / camera_zoom;

    auto& io = ImGui::GetIO();

    if (!io.WantCaptureKeyboard) {
        if (keyboard[SDL_SCANCODE_W]) {
            camera_position.y -= factor;
        }
        if (keyboard[SDL_SCANCODE_S]) {
            camera_position.y += factor;
        }
        if (keyboard[SDL_SCANCODE_A]) {
            camera_position.x += factor;
        }
        if (keyboard[SDL_SCANCODE_D]) {
            camera_position.x -= factor;
        }
    }

    camera_position = glm::clamp(camera_position, glm::vec2(-1.0f), glm::vec2(1.0f));

    view_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(camera_position, 1.0f));
    projection_matrix = orthographic_view(window_size.x, window_size.y, camera_zoom);
}
