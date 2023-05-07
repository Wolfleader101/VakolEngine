#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in vec4 aBoneIDs;
layout (location = 6) in vec4 aBoneWeights;

out VS_OUT
{
    vec3  normal;
    vec2  uv;
    vec3  tangent;
    vec3  bitangent;
    vec4  bone_ids;
    vec4  bone_weights;
} vs_out;

layout (std140, binding = 1) uniform Matrices
{
    mat4 PV_MATRIX;
    mat4 VIEW_MATRIX;
    mat4 MODEL_MATRIX;
};

void main()
{   
    vs_out.normal = aNormal;
    vs_out.uv = aTexCoords;
    vs_out.tangent = aTangent;
    vs_out.bitangent = aBitangent;
    vs_out.bone_ids = aBoneIDs;
    vs_out.bone_weights = aBoneWeights;

    gl_Position = PV_MATRIX * MODEL_MATRIX * vec4(aPos, 1.0);
}