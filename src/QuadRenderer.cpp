#include "QuadRenderer.h"

QuadRenderer::QuadRenderer()
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
}

QuadRenderer::~QuadRenderer()
{
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteVertexArrays(1, &vertex_array);
}

void QuadRenderer::draw()
{
    glBindVertexArray(vertex_array);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
