#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

layout(std140) uniform Matrices {
    mat4 projection;
    mat4 view;
};

uniform mat4 model = mat4(1.0);

void main() {
    gl_Position = projection * view * model * vec4(pos, 1.0);
    vec3 aNormal = mat3(inverse(transpose(model))) * normal;
}