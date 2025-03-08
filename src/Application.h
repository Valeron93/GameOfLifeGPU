#pragma once

#include "Camera.h"
#include "GameOfLife.h"
#include "QuadRenderer.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <imgui.h>

struct Application {
    SDL_Window* window = nullptr;
    SDL_GLContext gl_context = nullptr;

    uint64_t last_time;
    double delta_time;

    GLuint program = 0;

    QuadRenderer quad;

    glm::ivec2 window_size_px;
    glm::ivec2 window_size;

    GLint view_projection_matrix_location;
    GLint alive_cell_color_location;
    GLint dead_cell_color_location;

    const float camera_min_zoom = 1.0f;
    float camera_max_zoom = 64.0f;
    Camera cam;

    bool iterate = false;
    float iteration_timer = 0.0f;
    int iterations_per_sec = 30;

    int field_size = 1000;
    GameOfLife game_of_life;
    int max_texture_size;

    glm::vec4 alive_cell_color = glm::vec4(1.0f);
    glm::vec4 dead_cell_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    bool dragging = false;
    glm::vec2 last_drag_point = glm::vec2(0.0f);
    bool fullscreen = false;
    bool hide_gui = false;

    SDL_Cursor* pan_cursor = nullptr;

    Application(SDL_Window* window, SDL_GLContext gl_context);
    ~Application();

    void render();
    void imgui();
    void on_event(SDL_Event* event);
    void on_mouse_event(SDL_Event* event);
    void on_mouse_motion_event(SDL_Event* event);
    void on_keyboard_event(SDL_Event* event);
    void on_mouse_btn_event(SDL_Event* event);
    void on_mouse_wheel_event(SDL_Event* event);
    void update();
};