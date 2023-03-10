#version 460 core
out vec4 FragColor;

in vec3 TexCoords;

uniform vec4 rgba;

uniform samplerCube skybox;

uniform bool enableTexture;

void main()
{
    if (enableTexture)
        FragColor = texture(skybox, TexCoords) * rgba;
    else
        FragColor = rgba;
}