#version 330 core

out vec4 FragColor;

uniform sampler2D sampler;

in VS_OUT {
    vec2 texCoords;
} fs_in;

void main() {
    float depthValue = texture2D(sampler, fs_in.texCoords).r;
    FragColor = vec4(vec3(depthValue), 1.0);
}