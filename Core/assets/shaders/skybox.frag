#version 460 core
out vec4 FragColor;

in vec3 TexCoords;

uniform vec4 rgba;

uniform samplerCube skybox;

void main()
{
    FragColor = texture(skybox, TexCoords) * rgba;
}