#version 330 core

layout(location = 0) in vec3 pos;
out vec3 fragTexCoords;

uniform mat4 model = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 projection = mat4(1.0);

void main() {
    mat4 modifiedView = mat4(mat3(view));
    vec4 position = projection * modifiedView * model * vec4(pos, 1.0);
    gl_Position = position.xyww;
    fragTexCoords = pos;
}