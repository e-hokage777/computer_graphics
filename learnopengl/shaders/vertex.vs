#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoordsVec;

out VS_OUT {
    vec2 texCoords;
    vec3 normal;
    vec3 pos;
} vs_out;



uniform mat4 model = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 projection = mat4(1.0);
uniform mat4 transform = mat4(1.0);

void main() {
    vec4 worldPos = model * vec4(pos, 1.0);
    vs_out.pos = worldPos.xyz;
    vec4 vertexPos = projection * view * worldPos;
    gl_Position = vertexPos;
    vs_out.texCoords = texCoordsVec;
    vs_out.normal = normalize(mat3(transpose(inverse(model))) * normal);

}