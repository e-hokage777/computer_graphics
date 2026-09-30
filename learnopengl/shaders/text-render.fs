#version 330 core

in vec2 texCoords;
out vec4 FragColor;

uniform vec3 textColor;
uniform sampler2D sampler;

void main(){
    vec4 sampled = vec4(1.0f, 1.0f, 1.0f, texture(sampler, texCoords).r);
    FragColor = vec4(textColor, 1.0f) * sampled;
}