#version 460 core

// In variables
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

// Uniform variables
uniform mat4 PV_MATRIX;

// Out variables
out vec3 color;

void main() 
{
    gl_Position = PV_MATRIX * vec4(aPos, 1.0);

    // Transfer the vertex color to the fragment shader
    color = aColor;
}
