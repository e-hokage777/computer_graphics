#version 430 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

// outputs and inputs
out vec4 FragColor;
in vec2 fTexCoords;
in vec3 fNormal;
in vec3 fragPos;

// uniforms
uniform vec3 lightPos;
uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);
uniform sampler2D texture1;
uniform Material material;


// function declarations
vec3 computePointLight();

void main() {
    vec3 pointLight = computePointLight();
    FragColor = vec4(vec3(texture(texture1, fTexCoords)) * pointLight, 1.0);
}

vec3 computePointLight() {
    // normalizing normal vector
    vec3 norm = normalize(fNormal);
    // ambient strength
    float ambientStrength = 0.2;

    // diffuse strength
    vec3 lightDir = normalize(lightPos - fragPos);
    float diffuseStrength = max(dot(norm, lightDir), 0) ;

    // specular Strength;

    vec3 ambient = ambientStrength * material.ambient;
    vec3 diffuse = diffuseStrength * material.diffuse;

    return (ambient + diffuse) * lightColor;
    ;
}