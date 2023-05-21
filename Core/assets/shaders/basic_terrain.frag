#version 460 core
out vec4 FragColor;

in vec2 TexCoords;
in float Height;

uniform sampler2D diffuse_map; // optional texture

void main()
{
     //float h = (Height + 16.0) / 32.0; // greyscale value

     //FragColor = vec4(h, h, h, 1.0);

     FragColor = texture(diffuse_map, TexCoords * 20); // if you want to use textures, un-comment this
}