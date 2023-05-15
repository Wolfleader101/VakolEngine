#version 460 core
layout (location = 0) in vec3  aPos;
layout (location = 1) in vec3  aNormal;
layout (location = 2) in vec2  aTexCoords;
layout (location = 3) in vec3  aTangent;
layout (location = 4) in vec3  aBitangent;
layout (location = 5) in ivec4 aBoneIDs;
layout (location = 6) in vec4  aBoneWeights;

out VS_OUT
{
    vec3  fragPos;
    vec3  normal;
    vec2  uv;
} vs_out;

layout (std140, binding = 1) uniform Matrices
{
    mat4 PROJECTION_MATRIX;
    mat4 VIEW_MATRIX;
    mat4 PV_MATRIX;
    mat4 MODEL_MATRIX;
};

uniform mat3 NORMAL_MATRIX;

//layout (std430, binding = 2) buffer Bones
//{
//    mat4 BONE_TRANSFORMS[];
//};

uniform mat4 BONE_TRANSFORMS[100]; // TODO: Make this into a shader buffer storage

void main()
{  
    mat4 BONE_MATRIX = mat4(0.0f);

    for (int i = 0; i < 4; ++i)
         BONE_MATRIX += BONE_TRANSFORMS[aBoneIDs[i]] * aBoneWeights[i];

    if (aBoneIDs[0] < 0)
        BONE_MATRIX = mat4(1.0);

    vs_out.fragPos = vec3(MODEL_MATRIX * vec4(aPos, 1.0));
    vs_out.normal = NORMAL_MATRIX * aNormal;
    vs_out.uv = aTexCoords;

    gl_Position = PV_MATRIX * MODEL_MATRIX * BONE_MATRIX * vec4(aPos, 1.0);
}