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
    double avg_fps = 60;

    GLuint program = 0;

    QuadRenderer quad;

    glm::ivec2 window_size_px;
    glm::ivec2 window_size;

    GLint view_projection_matrix_location;

    const float camera_min_zoom = 1.0f;
    const float camera_max_zoom = 64.0f;
    Camera cam;

    bool iterate = false;
    float iteration_timer = 0.0f;
    int iterations_per_sec = 30;

    GameOfLife game_of_life;

    bool dragging = false;

    Application();
    ~Application();

    void render();
    void imgui();
    void on_event(SDL_Event* event);
    void update();
};