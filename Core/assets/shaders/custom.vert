#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 PV_MATRIX;
uniform mat3 NORMAL_MATRIX;
uniform mat4 MODEL_MATRIX;

void main()
{
    FragPos = vec3(MODEL_MATRIX * vec4(aPos, 1.0));
    Normal = NORMAL_MATRIX * aNormal;
    TexCoords = aTexCoords;

    gl_Position = PV_MATRIX * vec4(FragPos, 1.0);
}