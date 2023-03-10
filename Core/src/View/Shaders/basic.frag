#version 460 core
out vec4 FragColor;

uniform vec4 rgba;

in vec2 uv;

uniform sampler2D inputTexture0;
uniform float uvScale0 = 1.0;

uniform bool enableTexture;

void main()
{
    if (enableTexture)
        FragColor = texture(inputTexture0, uv * uvScale0) * rgba;
    else
        FragColor = rgba;
}