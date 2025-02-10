#include "GameOfLife.h"

GameOfLife::GameOfLife()
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

GLuint GameOfLife::get_texture() const
{
    return front_texture;
}

glm::vec2 GameOfLife::get_size() const
{
    return glm::vec2(width, height);
}

void GameOfLife::set_cell(int x, int y, bool value)
{
    if (x < 0 || x >= width) {
        return;
    }
    if (y < 0 || y >= height) {
        return;
    }

    GLubyte color[4] = { 0xFF, 0xFF, 0xFF, 0xFF };
    if (!value) {
        std::fill(color, color + 3, 0);
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, back_texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, front_texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
}

bool GameOfLife::get_cell(int x, int y) const
{
    if (x < 0 || x >= width) {
        return false;
    }
    if (y < 0 || y >= height) {
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, front_texture, 0);
    GLubyte pixel[4];
    glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return bool(pixel[0]);
}

void GameOfLife::iterate()
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
