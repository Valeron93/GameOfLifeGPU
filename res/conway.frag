#version 330 core
out vec4 FragColor;
in vec2 uv;

uniform sampler2D tex;

vec2 texelSize = 1.0 / textureSize(tex, 0);

int getCell(vec2 coord) {
    return int(texture(tex, coord).r > 0.5);
}

void main()
{
    int currentState = getCell(uv);

    int neighbors = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;
            neighbors += getCell(uv + vec2(i, j) * texelSize);
        }
    }

    int nextState = currentState;
    if (currentState == 1 && (neighbors < 2 || neighbors > 3))
        nextState = 0;
    else if (currentState == 0 && neighbors == 3)
        nextState = 1;

    FragColor = vec4(vec3(nextState), 1.0);
}
