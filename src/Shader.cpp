#include "Shader.h"

#include <SDL3/SDL.h>
#include <string>

#include "util.h"

namespace shader {
GLuint load_from_text(const char* vertex_shader_source, const char* fragment_shader_source)
{

    auto program = glCreateProgram();
    auto vert_shader = compile_shader(GL_VERTEX_SHADER, vertex_shader_source);
    auto frag_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_shader_source);

    if (vert_shader == 0 || frag_shader == 0) {
        glDeleteProgram(program);
        SDL_LogError(SDL_LOG_CATEGORY_GPU, "Failed create program %d", program);
        return 0;
    }
    glAttachShader(program, vert_shader);
    glAttachShader(program, frag_shader);

    glLinkProgram(program);

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    int link_status;
    glGetProgramiv(program, GL_LINK_STATUS, &link_status);

    if (!link_status) {
        glDeleteShader(vert_shader);
        glDeleteShader(frag_shader);
        char info_log[512];
        glGetProgramInfoLog(program, sizeof(info_log), nullptr, info_log);
        SDL_LogError(SDL_LOG_CATEGORY_GPU, "Failed to link program: %s", info_log);
        return 0;
    }
    return program;
}

GLuint compile_shader(GLenum shader_type, const char* source)
{
    auto shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    int compile_status = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);

    if (!compile_status) {
        char info_log[512];
        glGetShaderInfoLog(shader, sizeof(info_log), nullptr, info_log);
        SDL_LogError(SDL_LOG_CATEGORY_GPU, "Failed to compile shader: %s", info_log);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint load_path(const char* vertex_path, const char* fragment_path)
{
    auto vert_src = util::read_all_text(vertex_path);
    auto frag_src = util::read_all_text(fragment_path);

    return load_from_text(vert_src.c_str(), frag_src.c_str());
}

}