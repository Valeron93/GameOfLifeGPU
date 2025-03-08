#version 330 core

out vec4 FragColor;

uniform sampler2D tex;
uniform vec4 alive_cell_color;
uniform vec4 dead_cell_color;

in vec2 uv;

void main()
{
    float state = texture(tex, uv).r;
    FragColor = mix(dead_cell_color, alive_cell_color, state);
} 