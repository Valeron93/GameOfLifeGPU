#pragma once

#include "GameOfLife.h"
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

    GLuint vertex_array = 0;
    GLuint vertex_buffer = 0;
    GLuint program = 0;

    glm::ivec2 window_size_px;
    glm::ivec2 window_size;

    GLint view_matrix_location;
    GLint projection_matrix_location;

    glm::mat4 view_matrix;
    glm::mat4 projection_matrix;

    glm::vec2 camera_position = glm::vec2(0.0f);
    float camera_zoom = 1.0f;

    bool iterate = false;

    GameOfLife game_of_life;

    Application();
    ~Application();

    void render();
    void imgui();
    void on_event(SDL_Event* event);
    void update();
};