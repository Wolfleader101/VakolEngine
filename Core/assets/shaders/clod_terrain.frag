#version 460 core

out vec4 FragColor;

uniform sampler2D height_map;

in float height;
in vec2 TexCoord;

uniform float texel_size = 1.0 / 2048.0;

void main()
{
    const float HEIGHT_SCALE = 1.0;
    
    // float left  = texture(height_map, TexCoord + vec2(-texel_size, 0.0)).r * HEIGHT_SCALE * 2.0 - 1.0;
    // float right = texture(height_map, TexCoord + vec2( texel_size, 0.0)).r * HEIGHT_SCALE * 2.0 - 1.0;
    // float up    = texture(height_map, TexCoord + vec2(0.0,  texel_size)).r * HEIGHT_SCALE * 2.0 - 1.0;
    // float down  = texture(height_map, TexCoord + vec2(0.0, -texel_size)).r * HEIGHT_SCALE * 2.0 - 1.0;
    // vec3 normal = normalize(vec3(down - up, 2.0, left - right));

    float h = (height + 16) / 64.0;

    FragColor = vec4(h, h, h, 1.0);
}