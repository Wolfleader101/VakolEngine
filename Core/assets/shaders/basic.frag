#version 460 core
out vec4 FragColor;

in float height;
in vec2 TexCoords;

uniform sampler2D texture_0;

uniform bool enable_textures = true;

void main()
{
     if (enable_textures)
          FragColor = texture(texture_0, TexCoords);
     else
     {
          //float h = (height + 16.0) / 32.0;
          //FragColor = vec4(h, h, h, 1.0);
          FragColor = vec4(1.0);
     }
}