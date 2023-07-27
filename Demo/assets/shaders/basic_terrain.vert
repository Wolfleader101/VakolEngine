#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

// interface block
out VS_OUT 
{
    vec2 TexCoords;
} vs_out;

out float Height;

layout (std140, binding = 1) uniform Matrices
{
    mat4 PROJECTION_MATRIX;
    mat4 VIEW_MATRIX;
    mat4 PV_MATRIX;
};

uniform mat4 MODEL_MATRIX;

void main()
{   
    vs_out.TexCoords = aTexCoords;

    Height = ((aPos.y + 16) / 64.0) * 255.0;

    gl_Position = PV_MATRIX * MODEL_MATRIX * vec4(aPos, 1.0);
}