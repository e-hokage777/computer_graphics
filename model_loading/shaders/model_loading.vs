#version 430 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

out vec2 fTexCoords;
out vec3 fNormal;
out vec3 fragPos;

uniform mat4 model = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 projection = mat4(1.0);

void main() {
    gl_Position = projection * view * model * vec4(pos, 1.0);
    fTexCoords = texCoords;
    fNormal = mat3(transpose(inverse(model))) * normal;
    fragPos = vec3(model * vec4(pos, 1.0));
}