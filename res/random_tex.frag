#version 330 core

in vec2 uv;
out vec4 FragColor;

uniform float random_seed;

float random(vec2 uv, float seed)
{
    return fract(sin(dot(uv * seed, vec2(127.1, 311.7))) * 43758.5453123);
}

void main()
{
    float random_value = random(uv, float(random_seed));
    int value = 0;

    if (random_value > 0.8f) {
        value = 1;
    }

    FragColor = vec4(vec3(float(value)), 1.0f);
}