#version 330 core

in float fragment_color;
out vec3 color;

uniform sampler1D sampler;

void main() {
    float b = texture(sampler, fragment_color).b;
    color = vec3(0, 1.5f * b, 1.5f * b);
}