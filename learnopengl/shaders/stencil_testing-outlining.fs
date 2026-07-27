#version 330 core


out vec4 FragColor;
vec3 baseColor = vec3(0.17, 0.6, 0.27);


void main() {
    FragColor = vec4(baseColor, 1.0);

}