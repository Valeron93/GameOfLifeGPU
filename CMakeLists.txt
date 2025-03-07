cmake_minimum_required(VERSION 3.21)

project(GameOfLife VERSION 0.0.6)

include(FetchContent)

set(SDL_STATIC ON)

FetchContent_Declare(
    SDL3
    URL https://github.com/libsdl-org/SDL/archive/refs/tags/release-3.2.8.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_Declare(
    imgui
    URL https://github.com/ocornut/imgui/archive/refs/tags/v1.91.8.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_Declare(
    glad41core
    URL https://github.com/Valeron93/glad41core/archive/refs/heads/master.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_Declare(
    glm
    URL https://github.com/g-truc/glm/archive/refs/tags/1.0.1.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_Declare(
    stb
    URL https://github.com/nothings/stb/archive/5c205738c191bcb0abc65c4febfa9bd25ff35234.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(SDL3 imgui glad41core glm stb)

add_executable(GameOfLife WIN32
    "src/main.cpp"
    "src/Application.cpp"
    "src/Application.h"
    "src/Shader.cpp"
    "src/Shader.h"
    "src/util.cpp"
    "src/util.h"
    "src/GameOfLife.cpp"
    "src/GameOfLife.h"
    "src/QuadRenderer.cpp"
    "src/QuadRenderer.h"
    "src/Camera.cpp"
    "src/Camera.h"
    "src/stb_impl.c"
    "${imgui_SOURCE_DIR}/imgui.cpp"
    "${imgui_SOURCE_DIR}/imgui_demo.cpp"
    "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
    "${imgui_SOURCE_DIR}/imgui_tables.cpp"
    "${imgui_SOURCE_DIR}/imgui_draw.cpp"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp")

target_include_directories(GameOfLife PUBLIC ${imgui_SOURCE_DIR})
target_compile_definitions(GameOfLife PRIVATE GLFW_INCLUDE_NONE)

set_target_properties(GameOfLife PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_link_libraries(GameOfLife PUBLIC SDL3::SDL3-static glad41core glm)
target_include_directories(GameOfLife PUBLIC ${stb_SOURCE_DIR})

install(TARGETS GameOfLife DESTINATION .)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/res DESTINATION .)

set(CPACK_GENERATOR "ZIP")
include(CPack)
