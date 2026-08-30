#version 330 core

out vec4 FragColor;
// uniform vec3 lightColor;
// uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform bool blinn;
uniform vec3 lightPositions[1];
uniform vec3 lightColors[1];
uniform mat4 lightSpaceMatrix;
uniform sampler2D shadowMap;

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

struct DirectinalLight {
    vec3 position;
    vec3 direction;
    vec3 color;
};

uniform Material material;
uniform DirectinalLight dirLight;

// globals
vec3 diffuseTexture;

float computeDiffuseStrength(vec3 lightDir, vec3 normal) {

    float diffuseStrength = max(dot(normal, -lightDir), 0);
    return diffuseStrength;
}

float computeSpecularStrength(vec3 lightDir, vec3 viewDir, vec3 normal) {

    vec3 reflectedLight = reflect(lightDir, normal);

    float specularStrength = max(dot(-viewDir, reflectedLight), 0);

    specularStrength = pow(specularStrength, 128);

    return specularStrength;
}

float computeBlinnPhongSpecularStrength(vec3 lightDir, vec3 viewDir, vec3 normal) {
    vec3 midVector = normalize(-lightDir - viewDir);

    float specularStrength = max(dot(normal, midVector), 0);

    specularStrength = pow(specularStrength, 128);

    return specularStrength;
}

float computeAttenuation(vec3 lightPos) {
    float dist = distance(frag_in.pos, lightPos);
    float attenuation = 1 / (dist);

    return attenuation;
}

vec3 computeDirectionalLight(vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(dirLight.direction);
    float attenuation = 1;
    float diffuseStrength = computeDiffuseStrength(lightDir, normal);
    vec3 diffuse = diffuseTexture * diffuseStrength * attenuation * dirLight.color;

    float specularStrength;

    if(blinn) {
        specularStrength = computeBlinnPhongSpecularStrength(lightDir, viewDir, normal);
    } else {
        specularStrength = computeSpecularStrength(lightDir, viewDir, normal);
    }

    vec3 specular = diffuseTexture * specularStrength * attenuation * dirLight.color;

    return diffuse + specular;
}

float computeShadow(vec3 lightDir) {
    vec4 fragPosLightSpace = lightSpaceMatrix * vec4(frag_in.pos, 1.0);
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float shadowDepth = texture2D(shadowMap, projCoords.xy).r;

    // if(projCoords.z < 0) return 1.0;

    float bias = max(0.05 * (1 - dot(lightDir, frag_in.normal)), 0.005) * 0;
    return shadowDepth < projCoords.z - bias ? 1.0 : 0.0;

}

float pcfShadow() {
    vec4 fragPosLightSpace = lightSpaceMatrix * vec4(frag_in.pos, 1.0);
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    for(int i = -1; i <= 1; ++i){
        
    }
}

void main() {
    diffuseTexture = vec3(texture2D(material.texture_diffuse1, frag_in.texCoords));
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
        float attenuation = computeAttenuation(lightPositions[i]);
        diffuseStrength = computeDiffuseStrength(lightDir, normal) * 3;
        diffuse += diffuseTexture * diffuseStrength * attenuation * lightColors[i];

        if(blinn) {
            specularStrength = computeBlinnPhongSpecularStrength(lightDir, viewDir, normal);
        } else {
            specularStrength = computeSpecularStrength(lightDir, viewDir, normal);
        }

        specular += diffuseTexture * specularStrength * attenuation * lightColors[i];

    }

    // float attenuation = computeAttenuation(lightPositions[i]);

    //// computing shadow
    float shadow = computeShadow(dirLight.position - frag_in.pos);

    //// computing lights
    // ambient
    float ambientStrength = 0.2;
    // diffuse
    // specular

    vec3 directionalLight = computeDirectionalLight(normal, viewDir);

    vec3 ambient = diffuseTexture * ambientStrength;

    vec3 color = ambient + (diffuse + specular) * (1 - shadow);
    float gamma = 2.2;
    // FragColor = vec4(pow(color, vec3(1 / gamma)), 1.0);
    FragColor = vec4(color, 1.0);
}
