#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FinalColor;
out vec2 uv;

struct Light
{
    vec3 position;
    vec3 color;
};

struct Strength
{
    float ambient;
    float specular;
};

uniform Light light;
uniform Strength strength;

uniform int shininess = 32;

uniform vec3 viewPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform mat3 normalMatrix;

void main()
{
    uv = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    vec3 position = vec3(model * vec4(aPos, 1.0));
    vec3 normal = normalMatrix * aNormal;

    // ambient
    vec3 ambient = strength.ambient * light.color;

    // diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.color;

    // specular
    vec3 viewDir = normalize(viewPos - position);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = strength.specular * spec * light.color;

    FinalColor = ambient + diffuse + specular;
}