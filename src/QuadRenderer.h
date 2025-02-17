#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

struct Vertex {
    glm::vec2 position;
    glm::vec2 uv;
};

struct QuadRenderer {

    GLuint vertex_array;
    GLuint vertex_buffer;

    QuadRenderer();
    ~QuadRenderer();

    void draw();
};