#version 330 core

out vec4 FragColor;

uniform vec3 baseColor = vec3(0.776, 0.765, 0.710);

float near = 0.1;
float far = 100.0;
float LinearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main() {
    // FragColor = vec4(baseColor, 1.0);
    float depth = LinearizeDepth(gl_FragCoord.z) / far; // / far for demo
    FragColor = vec4(vec3(depth), 1.0);

}