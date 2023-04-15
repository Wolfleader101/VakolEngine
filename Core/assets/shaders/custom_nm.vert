#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

out VS_OUT
{
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} vs_out;

uniform mat4 PV_MATRIX;
uniform mat3 NORMAL_MATRIX;
uniform mat4 MODEL_MATRIX;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    vs_out.FragPos = vec3(MODEL_MATRIX * vec4(aPos, 1.0));
    vs_out.TexCoords = aTexCoords;

    vec3 T = normalize(NORMAL_MATRIX * aTangent); // tangent
    vec3 N = normalize(NORMAL_MATRIX * aNormal); // normal

    T = normalize(T - dot(T, N) * N); // not sure
    vec3 B = cross(N, T);

    mat3 TBN = transpose(mat3(T, B, N));
    vs_out.TangentLightPos = TBN * lightPos;
    vs_out.TangentViewPos =  TBN * viewPos;
    vs_out.TangentFragPos =  TBN * vs_out.FragPos;

    gl_Position = PV_MATRIX * MODEL_MATRIX * vec4(aPos, 1.0);
}