#version 330 core

out vec4 FragColor;
uniform vec3 cameraPos;
uniform bool blinn;
uniform vec3 lightPositions[1];
uniform vec3 lightColors[1];
uniform mat4 lightSpaceMatrix;
uniform sampler2D shadowMap;
uniform float farPlane;

in VS_OUT {
    vec2 texCoords;
    vec3 normal;
    vec3 pos;
} frag_in;

in vec4 vertexPos;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_diffuse3;
    sampler2D texture_specular1;
    sampler2D texture_specular2;
};

uniform Material material;
uniform samplerCube depthCubeMap;

// globals
vec3 diffuseTexture;

float computeDiffuseStrength(vec3 lightDir, vec3 normal) {

    float diffuseStrength = max(dot(normal, lightDir), 0);
    return diffuseStrength;
}

float computeSpecularStrength(vec3 lightDir, vec3 viewDir, vec3 normal) {

    vec3 reflectedLight = reflect(-lightDir, normal);

    float specularStrength = max(dot(viewDir, reflectedLight), 0);

    specularStrength = pow(specularStrength, 128);

    return specularStrength;
}

float computeBlinnPhongSpecularStrength(vec3 lightDir, vec3 viewDir, vec3 normal) {
    vec3 midVector = normalize(lightDir + viewDir);

    float specularStrength = max(dot(normal, midVector), 0);

    specularStrength = pow(specularStrength, 128);

    return specularStrength;
}

float computeAttenuation(vec3 lightPos) {
    float dist = distance(frag_in.pos, lightPos);
    float attenuation = 1 / (dist);

    return attenuation;
}

// vec3 computeDirectionalLight(vec3 normal, vec3 viewDir) {
//     vec3 lightDir = normalize(-dirLight.direction);
//     float attenuation = 1;
//     float diffuseStrength = computeDiffuseStrength(lightDir, normal);
//     vec3 diffuse = diffuseTexture * diffuseStrength * attenuation * dirLight.color;

//     float specularStrength;

//     if(blinn) {
//         specularStrength = computeBlinnPhongSpecularStrength(lightDir, viewDir, normal);
//     } else {
//         specularStrength = computeSpecularStrength(lightDir, viewDir, normal);
//     }

//     vec3 specular = diffuseTexture * specularStrength * attenuation * dirLight.color;

//     return diffuse + specular;
// }

vec3 computePointLight(vec3 lightPos, vec3 normal, vec3 viewDir, vec3 color) {
    vec3 lightDir = normalize(lightPos - frag_in.pos);

    // attenuation
    float attenutaion = computeAttenuation(lightPos);

    float diffuseStrength;
    float specularStrength;

    // diffuse
    diffuseStrength = dot(lightDir, normal);

    if(blinn) {
        specularStrength = computeBlinnPhongSpecularStrength(lightDir, viewDir, normal);
    } else {
        specularStrength = computeSpecularStrength(lightDir, viewDir, normal);
    }

    vec3 diffuse = diffuseStrength * diffuseTexture * color;
    vec3 specular = specularStrength * diffuseTexture * color;

    attenutaion = 1; // remove later

    return (diffuse + specular) * attenutaion * 2;

}

float computePointLightShadows(vec3 lightDir) {
    float closestDepth = texture(depthCubeMap, -lightDir).r;

    float fragmentDepth = distance(frag_in.pos.xyz, lightPositions[0]);
    fragmentDepth = fragmentDepth / farPlane;

    float bias = max(0.05 * (1 - dot(-lightDir, frag_in.normal)), 0.005);

    return closestDepth  < fragmentDepth-bias ? 1.0 : 0.0;
}

float pcfShadow(vec3 lightDir) {
    vec4 fragPosLightSpace = lightSpaceMatrix * vec4(frag_in.pos, 1.0);
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);

    float shadow = 0;

    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            vec2 coord = projCoords.xy + vec2(x, y) * texelSize;
            float shadowDepth = texture2D(shadowMap, coord).r;

            float bias = max(0.05 * (1 - dot(lightDir, frag_in.normal)), 0.005);
            shadow += shadowDepth < projCoords.z - bias ? 1.0 : 0.0;
        }
    }

    return shadow / 9.0;
}

void main() {
    diffuseTexture = vec3(texture2D(material.texture_diffuse1, frag_in.texCoords));
    vec3 specularTexture = vec3(texture2D(material.texture_specular1, frag_in.texCoords));

    // computing directions
    vec3 viewDir = normalize(cameraPos - frag_in.pos);
    vec3 normal = normalize(frag_in.normal);

    //// computing shadow
    // float shadow = computeShadow(frag_in.pos - dirLight.position);
    // float shadow = pcfShadow(frag_in.pos - dirLight.position);

    float pointLightShadow = computePointLightShadows(normalize(lightPositions[0] - frag_in.pos));

    //// computing lights
    // ambient
    float ambientStrength = 0.5;
    // diffuse
    // specular

    // vec3 directionalLight = computeDirectionalLight(normal, viewDir);
    vec3 pointLight = computePointLight(lightPositions[0], normal, viewDir, lightColors[0]);

    vec3 ambient = diffuseTexture * ambientStrength;

    vec3 color = ambient + pointLight * (1.0 - pointLightShadow);
    // float gamma = 2.2;
    // FragColor = vec4(pow(color, vec3(1 / gamma)), 1.0);
    FragColor = vec4(color, 1.0);
}
