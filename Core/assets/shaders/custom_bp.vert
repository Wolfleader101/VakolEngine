#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

// interface block
out VS_OUT {
    vec4 FragCoords;
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

layout (std140, binding = 1) uniform Matrices
{
    mat4 PV_MATRIX;
    mat4 VIEW_MATRIX;
};

uniform mat3 NORMAL_MATRIX;
uniform mat4 MODEL_MATRIX;

uniform bool enable_fog = false;

void main()
{
    if (enable_fog)
        vs_out.FragCoords = VIEW_MATRIX * MODEL_MATRIX * vec4(aPos, 1.0);

    vs_out.FragPos = vec3(MODEL_MATRIX * vec4(aPos, 1.0));
    vs_out.Normal = NORMAL_MATRIX * aNormal;
    vs_out.TexCoords = aTexCoords;
    
    gl_Position = PV_MATRIX * MODEL_MATRIX * vec4(aPos, 1.0);

}