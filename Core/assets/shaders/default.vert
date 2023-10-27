#version 460 core
layout (location = 0) in vec3  aPos;
layout (location = 1) in vec3  aNormal;
layout (location = 2) in vec2  aTexCoords;
layout (location = 3) in vec3  aTangent;
layout (location = 4) in vec3  aBitangent;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 Normal;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} vs_out;

uniform mat4 PV_MATRIX;
uniform mat4 MODEL_MATRIX;
uniform mat3 NORMAL_MATRIX;

struct Light
{
    vec4 LIGHT_DIRECTION;
};

uniform Light light;

uniform vec3 VIEW_POSITION;

void main()
{
    vs_out.FragPos = vec3(MODEL_MATRIX * vec4(aPos, 1.0));
    vs_out.TexCoords = aTexCoords;

    vec3 T = normalize(NORMAL_MATRIX * aTangent);
    vec3 N = normalize(NORMAL_MATRIX * aNormal);

    vs_out.Normal = N;
    
    T = normalize(T - dot(T, N) * N);

    vec3 B = cross(N, T);

    mat3 TBN = transpose(mat3(T, B, N));
    vs_out.TangentLightPos =  TBN * light.LIGHT_DIRECTION.xyz;
    vs_out.TangentViewPos  =  TBN * VIEW_POSITION;
    vs_out.TangentFragPos  =  TBN * vs_out.FragPos;

	gl_Position = PV_MATRIX * MODEL_MATRIX * vec4(aPos, 1.0);
}