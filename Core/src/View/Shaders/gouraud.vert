#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec4 FinalColor;

typedef struct Light
{
    uniform vec3 position;
    uniform vec4 color;
}

typedef struct Strength
{
    uniform float ambient = 0.1;
    uniform float specular = 1.0;
}

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform mat3 normalMatrix;

void main()
{
    gl_Position = projection * view * model;

    vec3 position = vec3(model * vec4(aPos, 1.0));
    vec3 normal = normalMatrix * aNormal;

    vec3 ambient = Strength.ambient * Light.color;
}