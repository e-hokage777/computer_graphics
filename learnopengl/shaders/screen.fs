#version 330 core

out vec4 FragColor;

uniform sampler2D sampler;

in VS_OUT {
    vec2 texCoords;
} fs_in;

void main() {
    FragColor = texture2D(sampler, fs_in.texCoords);
}