#version 330 core

out vec4 FragColor;
// uniform vec3 lightColor;
// uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform bool blinn;
uniform vec3 lightPositions[3];
uniform vec3 lightColors[3];

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

float computeDiffuseStrength(vec3 lightDir, vec3 normal) {

    float diffuseStrength = max(dot(normal, -lightDir), 0);
    return diffuseStrength;
}

float computeSpecularStrength(vec3 lightDir, vec3 viewDir, vec3 normal) {

    vec3 reflectedLight = reflect(lightDir, normal);

    float specularStrength = max(dot(-viewDir, reflectedLight), 0);

    specularStrength = pow(specularStrength, 8);

    return specularStrength;
}

float computeBlinnPhongSpecularStrength(vec3 lightDir, vec3 viewDir, vec3 normal) {
    vec3 midVector = normalize(-lightDir - viewDir);

    float specularStrength = max(dot(normal, midVector), 0);

    specularStrength = pow(specularStrength, 32);

    return specularStrength;
}

void main() {
    vec3 diffuseTexture = vec3(texture2D(material.texture_diffuse1, frag_in.texCoords));
    vec3 specularTexture = vec3(texture2D(material.texture_specular1, frag_in.texCoords));

    // computing directions
    vec3 viewDir = normalize(frag_in.pos - cameraPos);
    vec3 normal = normalize(frag_in.normal);

    // lights
    float diffuseStrength;
    float specularStrength;
    vec3 diffuse = vec3(0);
    vec3 specular = vec3(0);

    for(int i = 0; i < 1; ++i) {
        vec3 lightDir = normalize(frag_in.pos - lightPositions[i]);
        diffuseStrength = computeDiffuseStrength(lightDir, normal);
        diffuse += diffuseTexture * diffuseStrength * lightColors[i];

        if(blinn) {
            specularStrength = computeBlinnPhongSpecularStrength(lightDir, viewDir, normal);
        } else {
            specularStrength = computeSpecularStrength(lightDir, viewDir, normal);
        }

        specular += diffuseTexture * specularStrength * lightColors[i];

    }

    //// computing lights
    // ambient
    float ambientStrength = 0.4;
    // diffuse
    // specular

    vec3 ambient = diffuseTexture * ambientStrength;

    vec3 color = ambient + diffuse + specular;

    FragColor = vec4(color, 1.0);
}
