#version 460 core
out vec4 FragColor;

in VS_OUT 
{
    vec2 TexCoords;
} fs_in;

in float Height;

uniform vec2 uv_scale = vec2(1.0);
uniform sampler2D layer_1;

void main()
{
    // vec4 lighting = vec4(texture(light_map, fs_in.TexCoords * vec2(1.0, 1.0)).rrr, 1.0);

   vec4 color = texture(layer_1, fs_in.TexCoords * uv_scale);

    

    FragColor = color;
}