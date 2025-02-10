#version 330 core

layout (location = 0) in vec2 vertex_position;
layout (location = 1) in vec2 uv_in;

out vec2 uv;

void main() {
    gl_Position = vec4(vertex_position, 0.0f, 1.0);
    uv = uv_in;
}