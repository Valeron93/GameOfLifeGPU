#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>

#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>

#include <glad/glad.h>

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl3.h>
#include <imgui.h>

#include "Application.h"

SDL_AppResult SDL_AppInit(void** state, int argc, char** argv)
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        return SDL_APP_FAILURE;
    }

    const auto window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_HIGH_PIXEL_DENSITY;
    auto* window = SDL_CreateWindow("Game of Life", 800, 800, window_flags);
    SDL_SetWindowMinimumSize(window, 300, 300);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    auto gl_context = SDL_GL_CreateContext(window);

    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

    ImGui::CreateContext();
    ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init();

    auto* app = new Application;
    app->window = window;
    app->gl_context = gl_context;
    app->last_time = SDL_GetPerformanceCounter();
    *state = app;

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* state)
{
    auto& app = *(Application*)state;

    auto current_time = SDL_GetPerformanceCounter();
    app.delta_time = double((current_time - app.last_time)) / double(SDL_GetPerformanceFrequency());

    app.update();

    app.render();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    app.imgui();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    SDL_GL_SwapWindow(app.window);
    app.last_time = current_time;
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* state, SDL_Event* event)
{
    ImGui_ImplSDL3_ProcessEvent(event);
    ((Application*)state)->on_event(event);
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* state, SDL_AppResult result)
{
    auto& app = *(Application*)state;
    ImGui_ImplSDL3_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyWindow(app.window);
    SDL_Quit();
}
