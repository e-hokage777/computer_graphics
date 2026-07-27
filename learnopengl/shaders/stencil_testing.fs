#version 330 core


out vec4 FragColor;
uniform vec3 baseColor = vec3(0.776, 0.765, 0.710);


void main() {
    FragColor = vec4(baseColor, 1.0);

}