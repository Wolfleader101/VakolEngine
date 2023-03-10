#version 460 core
out vec4 FragColor;

in vec2 uv;

uniform vec4 rgba;

uniform samplerCube inputTexture0;

uniform bool enableTexture;

void main()
{
    if (enableTexture)
        FragColor = texture(inputTexture0, uv) * rgba;
    else
        FragColor = rgba;
}