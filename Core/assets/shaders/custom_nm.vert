#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

// declare an interface block; see 'Advanced GLSL' for what these are.
out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

uniform mat4 PROJECTION_MATRIX;
uniform mat4 VIEW_MATRIX;
uniform mat4 MODEL_MATRIX;

void main()
{
    vs_out.FragPos = aPos;
    vs_out.Normal = aNormal;
    vs_out.TexCoords = aTexCoords;
    
    gl_Position = PROJECTION_MATRIX * VIEW_MATRIX * vec4(aPos, 1.0);
}