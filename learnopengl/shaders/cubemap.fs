#version 330 core

out vec4 FragColor;

in vec3 fragTexCoords;
uniform samplerCube sampler;

void main() {
    FragColor = texture(sampler, fragTexCoords);
    // FragColor = vec4(1.0, 0.0, 0.01, 1.00);
}