#version 450

// Input from Vertex Shader
layout(location = 0) in vec3 fragColor;

// Output to the SwapChain (Screen)
layout(location = 0) out vec4 outColor;

void main() {
    // Output the color with alpha set to 1.0 (fully opaque)
    outColor = vec4(fragColor, 1.0);
}