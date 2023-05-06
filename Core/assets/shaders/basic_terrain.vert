#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;
out float Height;

layout (std140, binding = 1) uniform Matrices
{
    mat4 PV_MATRIX;
    mat4 VIEW_MATRIX;
    mat4 MODEL_MATRIX;
};

void main()
{   
    TexCoords = aTexCoords;
    Height = aPos.y;

    gl_Position = PV_MATRIX * MODEL_MATRIX * vec4(aPos, 1.0);
}