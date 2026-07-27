#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoordsVec;
// layout(location = 3) in vec3 instanceTranslate;
// layout(location = 4) in vec3 scale;
// layout(location = 5) in float rotation;
layout(location = 3) in mat4 instanceTransform;

out VS_OUT {
    vec2 texCoords;
    vec3 normal;
} vs_out;

uniform mat4 model = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 projection = mat4(1.0);
uniform mat4 transform = mat4(1.0);

void main() {
    // vec3 p = rotate(pos * scale, rotation) + instanceTranslate;
    gl_Position = projection * view * model * instanceTransform * vec4(pos, 1.0);
    vs_out.texCoords = texCoordsVec;
    vs_out.normal = mat3(transpose(inverse(model * instanceTransform))) * normal;

}