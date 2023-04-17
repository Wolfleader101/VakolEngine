#version 460 core
layout (location = 0) in vec3 aPos;

layout (std140, binding = 1) uniform Matrices
{
    mat4 PROJECTION_MATRIX;
    mat4 VIEW_MATRIX;
};

uniform mat4 MODEL_MATRIX;

void main()
{   
    gl_Position = PV_MATRIX * MODEL_MATRIX * vec4(aPos, 1.0);
}