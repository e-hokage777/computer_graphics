#version 330 core

out vec4 FragColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

in VS_OUT {
    vec2 texCoords;
    vec3 normal;
    vec3 pos;
} frag_in;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_diffuse3;
    sampler2D texture_specular1;
    sampler2D texture_specular2;
};

uniform Material material;

vec3 computeDiffuseLight() {
    vec3 lightDir = lightPos - frag_in.pos;
    float diffuseStrength = dot(frag_in.normal, -lightDir);
    return diffuseStrength * lightColor;
}

void main() {
    vec3 diffuse = vec3(texture2D(material.texture_diffuse1, frag_in.texCoords));
    vec3 specular = vec3(texture2D(material.texture_specular1, frag_in.texCoords));


    // computing lights
    vec3 diffuseLight = computeDiffuseLight();

    vec3 result = diffuse;
    FragColor = vec4(result, 1.0);
}