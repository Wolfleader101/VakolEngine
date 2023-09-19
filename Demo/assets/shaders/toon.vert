#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 Normal;
} vs_out;

uniform mat3 NORMAL_MATRIX;

uniform mat4 PV_MATRIX;
uniform mat4 MODEL_MATRIX;

void main()
{
    vs_out.FragPos = vec3(MODEL_MATRIX * vec4(aPos, 1.0));
    vs_out.TexCoords = aTexCoord;

    vs_out.Normal = normalize(NORMAL_MATRIX * aNormal);

    gl_Position = PV_MATRIX * vec4(vs_out.FragPos, 1.0);
}
