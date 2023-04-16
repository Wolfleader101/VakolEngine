#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 PV;
uniform mat4 MODEL;

void main()
{
    FragPos = vec3(MODEL * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(MODEL))) * aNormal;
    TexCoords = aTexCoords;

    gl_Position = PV * vec4(FragPos, 1.0);
}