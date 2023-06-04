#version 460 core
// In variables
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

layout(std140, binding = 1) uniform Matrices
{
    mat4 PROJECTION_MATRIX;
    mat4 VIEW_MATRIX;
    mat4 PV_MATRIX;
    //mat4 MODEL_MATRIX;
};

uniform mat4 MODEL_MATRIX;

// Out variables
out vec3 color;

void main() 
{
    gl_Position = PV_MATRIX * vec4(aPos, 1.0);

    // Transfer the vertex color to the fragment shader
    color = aColor;
}