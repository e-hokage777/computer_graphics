#version 330 core

out vec4 FragColor;
in vec2 texCoordsFrag;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_diffuse3;
    sampler2D texture_specular1;
    sampler2D texture_specular2;
};

uniform Material material;

void main() {
    vec4 diffuse = texture2D(material.texture_diffuse1, texCoordsFrag);

    vec4 result = diffuse;
    FragColor = vec4(result);
}