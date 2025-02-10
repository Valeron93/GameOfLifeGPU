#version 330 core

out vec4 FragColor;

uniform sampler2D tex;

in vec2 uv;

void main()
{
    FragColor = texture(tex, uv);
    // FragColor = vec4(uv, 0.0, 1.0);
} 