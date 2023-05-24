#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D diffuse_map;
uniform float alphaCutoff = 0.75f;

void main()
{
    FragColor = texture(diffuse_map, TexCoords);

    if (FragColor.a < alphaCutoff)
    {
        discard;
    }
}