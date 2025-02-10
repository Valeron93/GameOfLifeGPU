#pragma once

#include "Shader.h"
#include <cstdlib>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

class GameOfLife {

    GLuint framebuffer = 0;
    GLuint front_texture = 0;
    GLuint back_texture = 0;

    int width = 1000;
    int height = 1000;

    GLuint conway_program = 0;

public:
    GameOfLife();

    // NOTE: valid VAO must be bound so we can render a quad
    // TODO: render own quad mesh
    void iterate();
    GLuint get_texture() const;
    glm::vec2 get_size() const;

    bool get_cell(int x, int y) const;
    void set_cell(int x, int y, bool value);
};