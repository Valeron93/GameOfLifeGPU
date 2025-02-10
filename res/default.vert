#version 330 core

layout (location = 0) in vec2 vertex_position;
layout (location = 1) in vec2 uv_in;

uniform mat4 view_matrix = mat4(1.0);
uniform mat4 projection_matrix = mat4(1.0);
out vec2 uv;

void main() {
    gl_Position = projection_matrix * view_matrix * vec4(vertex_position, 0.0f, 1.0);
    uv = uv_in;
}