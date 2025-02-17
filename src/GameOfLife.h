#pragma once

#include "QuadRenderer.h"
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

    void iterate(QuadRenderer& quad);
    GLuint get_texture() const;
    glm::vec2 get_size() const;

    bool get_cell(int x, int y) const;
    void set_cell(int x, int y, bool value);
};