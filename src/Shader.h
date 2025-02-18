#pragma once
#include <glad/glad.h>

namespace shader {
GLuint load_from_text(const char* vertex_shader_source, const char* fragment_shader_source);
GLuint compile_shader(GLenum shader_type, const char* source);
GLuint load_path(const char* vertex_path, const char* fragment_path);
}