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
    vec3  normal;
    vec2  uv;
    ivec4 bone_ids;
    vec4  bone_weights;
} vs_out;

layout (std140, binding = 1) uniform Matrices
{
    mat4 PROJECTION_MATRIX;
    mat4 VIEW_MATRIX;
    mat4 PV_MATRIX;
    mat4 MODEL_MATRIX;
};

//layout (std430, binding = 2) buffer Bones
//{
//    mat4 BONE_TRANSFORMS[];
//};

uniform mat4 BONE_TRANSFORMS[100];

void main()
{  
    mat4 S = mat4(0.0f);

    ivec4 bone_ids = aBoneIDs * 52;

    for (int i = 0; i < 4; ++i)
    {
        if (bone_ids[i] >= 0)
            S += (BONE_TRANSFORMS[bone_ids[i]] * aBoneWeights[i]);
    }

    if (bone_ids[0] < 0)
        S = mat4(1.0);

    vs_out.normal = aNormal;
    vs_out.uv = aTexCoords;

    vs_out.bone_ids = bone_ids;
    vs_out.bone_weights = aBoneWeights;

    gl_Position = PV_MATRIX * MODEL_MATRIX * S * vec4(aPos, 1.0);
}