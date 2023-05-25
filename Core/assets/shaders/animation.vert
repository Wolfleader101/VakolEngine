#version 460 core
layout (location = 0) in vec3  aPos;
layout (location = 1) in vec3  aNormal;
layout (location = 2) in vec2  aTexCoords;
layout (location = 3) in vec3  aTangent;
layout (location = 4) in vec3  aBitangent;
layout (location = 5) in ivec4 aBoneIDs;
layout (location = 6) in vec4  aBoneWeights;
layout (location = 7) in mat4  aInstanceMatrix;

out VS_OUT
{
    vec3  fragPos;
    vec3  normal;
    vec2  uv;
    mat3  TBN;
} vs_out;

layout (std140, binding = 1) uniform Matrices
{
    mat4 PROJECTION_MATRIX;
    mat4 VIEW_MATRIX;
    mat4 PV_MATRIX;
    mat4 MODEL_MATRIX;
};

layout (std430, binding = 3) buffer Bones
{
    mat4 BONE_TRANSFORMS[];
};

uniform bool instanced = false;

void main()
{  
    mat4 BONE_MATRIX = mat4(0.0f);

    for (int i = 0; i < 4; ++i)
         BONE_MATRIX += BONE_TRANSFORMS[aBoneIDs[i]] * aBoneWeights[i];

    // if no bones (for debug bind pose)
    if (aBoneIDs[0] < 0)
        BONE_MATRIX = mat4(1.0);

    vs_out.fragPos = vec3(MODEL_MATRIX * BONE_MATRIX * vec4(aPos, 1.0));
    vs_out.uv = aTexCoords;

    mat3 MS = mat3(MODEL_MATRIX * BONE_MATRIX);
    vec3 T = normalize(MS * aTangent);
    vec3 B = normalize(MS * aBitangent);
    vec3 N = normalize(MS * aNormal);

    vs_out.TBN = mat3(T, B, N);
    vs_out.normal = N;

    if (instanced)
        gl_Position = PV_MATRIX * aInstanceMatrix * BONE_MATRIX * vec4(aPos, 1.0);
    else
        gl_Position = PV_MATRIX * MODEL_MATRIX * BONE_MATRIX * vec4(aPos, 1.0);
}