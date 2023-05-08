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
    ivec4  bone_ids;
    vec4  bone_weights;
} vs_out;

layout (std140, binding = 1) uniform Matrices
{
    mat4 PROJECTION_MATRIX;
    mat4 VIEW_MATRIX;
    mat4 PV_MATRIX;
    mat4 MODEL_MATRIX;
};

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 BONE_TRANSFORMS[52];

void main()
{   
    vec4 total_position = vec4(0.0);

    ivec4 bone_ids = ivec4(aBoneIDs * 52);

    for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
    {
        if (bone_ids[i] == -1) continue;

        if (bone_ids[i] >= MAX_BONES)
        {
            total_position = vec4(aPos, 1.0);
            break;
        }

        vec4 local_position = BONE_TRANSFORMS[bone_ids[i]] * vec4(aPos, 1.0);

        total_position += local_position * aBoneWeights[i];
    }

    mat4 VIEW_MODEL = VIEW_MATRIX * MODEL_MATRIX;

    vs_out.normal = aNormal;
    vs_out.uv = aTexCoords;

    vs_out.bone_ids = bone_ids;
    vs_out.bone_weights = aBoneWeights;

    gl_Position = PROJECTION_MATRIX * VIEW_MODEL * total_position;
}