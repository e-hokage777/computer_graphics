#version 330 core

out vec4 FragColor;
in vec2 fragTexcoords;

in VS_OUT {
    vec3 color;
} fs_in;

uniform sampler2D sampler;

void main() {
    // FragColor = texture2D(sampler, fragTexcoords);
    // FragColor = vec4(fs_in.color, 1.0);
    FragColor = vec4(0.04, 0.93, 0.04, 1.0);
}