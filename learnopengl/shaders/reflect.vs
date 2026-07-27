#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoordsVec;

out vec2 texCoordsFrag;
out vec3 fragNormal;
out vec3 fragPos;

uniform mat4 model = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 projection = mat4(1.0);
uniform mat4 transform = mat4(1.0);

void main() {
    vec4 position = projection * view * model * transform * vec4(pos, 1.0);
    texCoordsFrag = texCoordsVec;
    fragNormal = mat3(transpose(inverse(model))) * normal;
    fragPos = vec3(position);
    gl_Position = position;
}