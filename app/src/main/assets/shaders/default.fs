#version 300 es
precision mediump float;

out vec4 outColor;
in vec2 texCoords;
in vec3 normal;

void main() {
    float l = 1.0 - length(texCoords - 0.5);
    outColor = vec4(l, 0.0, l, 1.0);
}