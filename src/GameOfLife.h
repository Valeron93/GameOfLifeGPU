#pragma once

#include "QuadRenderer.h"
#include "Shader.h"
#include <cstdlib>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <random>
#include <vector>

class GameOfLife {

    GLuint framebuffer = 0;
    GLuint front_texture = 0;
    GLuint back_texture = 0;

    int width = 1000;
    int height = 1000;

    GLuint conway_program = 0;
    GLuint random_texture_program = 0;
    GLint random_seed_location = 0;

    QuadRenderer quad;
    std::mt19937 random_algorithm;
    std::uniform_real_distribution<float> random_distribution;

public:
    GameOfLife();
    void iterate();
    GLuint get_texture() const;
    glm::vec2 get_size() const;

    bool get_cell(int x, int y) const;
    void set_cell(int x, int y, bool value);

    void randomize_texture();
    void clear_texture();
};