#version 330 core

out vec4 FragColor;

uniform vec3 baseColor = vec3(0.7, 0.66, 0.7);

void main() {
    FragColor = vec4(baseColor, 1.0);
}