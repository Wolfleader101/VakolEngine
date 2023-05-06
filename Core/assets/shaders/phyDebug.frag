#version 330 core

// In variables
in vec3 color;

// Out variables
out vec4 colorOut;

void main() {

   colorOut = vec4(color, 1.0);
}