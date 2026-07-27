#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoordsVec;

out VS_OUT {
    vec2 texCoords;
    vec3 normal;
} vs_out;



uniform mat4 model = mat4(1.0);
uniform mat4 view = mat4(1.0);
// uniform mat4 projection = mat4(1.0);
uniform mat4 transform = mat4(1.0);

void main() {
    vs_out.texCoords = texCoordsVec;
    mat3 normalMatrix = mat3(1.0);
    // normalMatrix = mat3(view)* mat3(transpose(inverse( model)));
    normalMatrix =  mat3(transpose(inverse(view * model)));
    vs_out.normal =  normalize(normalMatrix * normal);
    gl_Position = view * model * vec4(pos, 1.0);
}