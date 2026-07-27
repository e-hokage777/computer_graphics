#version 330 core

out vec4 FragColor;
in vec2 texCoordsFrag;
in vec3 fragNormal;
in vec3 fragPos;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_diffuse3;
    sampler2D texture_specular1;
    sampler2D texture_specular2;
};

uniform Material material;
uniform samplerCube texture_world;
uniform vec3 cameraPosition;

void main() {
    // computing reflected ray
    vec3 incidentRay = fragPos - cameraPosition;
    vec3 reflectedRay = reflect(incidentRay, fragNormal);

    vec4 textureWorld = texture(texture_world, reflectedRay);
    vec4 diffuse = texture2D(material.texture_diffuse1, texCoordsFrag);
    vec3 specular = vec3(texture2D(material.texture_specular1, texCoordsFrag));

    vec3 result = vec3(mix(textureWorld, diffuse, 0.8));
    FragColor = vec4(result, 1.0);
}