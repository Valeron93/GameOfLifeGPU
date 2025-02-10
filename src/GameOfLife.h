#pragma once

#include "Shader.h"
#include <cstdlib>
#include <glad/glad.h>
#include <vector>

class GameOfLife {

    GLuint framebuffer = 0;
    GLuint front_texture = 0;
    GLuint back_texture = 0;

    int width = 1000;
    int height = 1000;

    GLuint conway_program = 0;

public:
    GameOfLife()
    {
        conway_program = shader::load_path("res/conway");
        std::vector<unsigned char> randomCells(width * height * 4);
        for (size_t i = 0; i < width * height; i++) {
            bool alive = !(rand() % 3);
            randomCells[i * 4] = alive ? 255 : 0;
            randomCells[i * 4 + 1] = alive ? 255 : 0;
            randomCells[i * 4 + 2] = alive ? 255 : 0;
            randomCells[i * 4 + 3] = 255;
        }

        glGenFramebuffers(1, &framebuffer);
        glGenTextures(1, &front_texture);
        glGenTextures(1, &back_texture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, front_texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, randomCells.data());

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, back_texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, randomCells.data());

        int tex_uni2 = glGetUniformLocation(conway_program, "tex");
        glUseProgram(conway_program);
        glUniform1i(tex_uni2, 1);
    }

    // NOTE: valid VAO must be bound so we can render a quad
    // TODO: render own quad mesh
    void iterate()
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, front_texture);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, back_texture);

        glViewport(0, 0, width, height);

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, front_texture, 0);
        glUseProgram(conway_program);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        std::swap(front_texture, back_texture);
    }

    GLuint get_texture()
    {
        return front_texture;
    }

    void set_cell(int x, int y, bool value)
    {
        if (x < 0 || x >= width) {
            return;
        }
        if (y < 0 || y >= height) {
            return;
        }

        GLubyte color[4] = { 0xFF, 0xFF, 0xFF, 0xFF };

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, back_texture);
        glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, front_texture);
        glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
    }
    glm::vec2 get_size()
    {
        return glm::vec2(width, height);
    }
};