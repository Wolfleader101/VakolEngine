#version 460 core
out vec4 FragColor;

in vec3 FinalColor;
in vec2 uv;

uniform vec3 rgba;

uniform sampler2D texture0;
uniform bool enableTexture;

void main()
{
    if (enableTexture)
        FragColor = texture(texture0, uv) * vec4(FinalColor * rgba, 1.0);
    else
        FragColor = vec4(FinalColor * rgba, 1.0);
}