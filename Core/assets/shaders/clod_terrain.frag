#version 460 core

out vec4 FragColor;

uniform sampler2D height_map;

uniform sampler2D dirt_layer;
uniform sampler2D grass_layer;
uniform sampler2D rock_layer;

layout (std140, binding = 3) uniform Time
{
    float time;
};

in float height;
in vec2 TexCoord;

uniform sampler2D water_diffuse;
uniform sampler2D water_foam;

uniform vec4 water_tint = vec4(0.0, 0.4, 1.0, 1.0);

uniform float water_scale_0 = 0.5;
uniform float water_scale_1 = 0.5;

uniform float texture_speed_0 = 0.1;
uniform float texture_speed_1 = 0.08;

uniform float wobbleX = 1.4;
uniform float wobbleY = 1.7;

void main()
{
    vec4 water_tex_0 = texture(water_diffuse, vec2(TexCoord.x + (time * texture_speed_0), TexCoord.y + (cos(time) * texture_speed_0)) + cos(time * vec2(wobbleX, wobbleY) + TexCoord.xy * 10.0) * 0.02 * 1.0);
    vec4 water_tex_1 = texture(water_foam, vec2(TexCoord.x + (sin(time) * texture_speed_1), TexCoord.y + (time * texture_speed_1)) + cos(time * vec2(wobbleX, wobbleY) + TexCoord.xy * 10.0) * 0.02 * 1.0);

    float h = (height + 16) / 64.0;

    vec4 color = vec4(0.0);

    if (h < 0.25)
        color = water_tint + (water_tex_0 * water_scale_0) + (water_tex_1 * water_scale_1);
    else if (h < 0.40)
        color = mix(texture(grass_layer, TexCoord * 2.0), texture(dirt_layer, TexCoord * 2.5), 1 - h);
    else if (h < 0.75)
        color = mix(texture(rock_layer, TexCoord * 2.0), texture(grass_layer, TexCoord * 2.0), 1 - h);
    else if (h < 0.9)
        color = texture(rock_layer, TexCoord * 2.0);
    else
        color = vec4(1.0);

    color = pow(color, vec4(vec3(1.0 / 2.2), 1.0));
    
    FragColor = color * vec4(h, h, h, 1.0);
}