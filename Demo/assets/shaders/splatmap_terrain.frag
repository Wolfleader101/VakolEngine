#version 460 core
out vec4 FragColor;

in VS_OUT 
{
    vec2 TexCoords;
} fs_in;

in float Height;

uniform sampler2D water_layer_1;
uniform sampler2D water_layer_2;

uniform sampler2D light_map;

uniform sampler2D layer_1;
uniform sampler2D layer_2;
uniform sampler2D layer_3;
uniform sampler2D layer_4;

uniform sampler2D noise;
uniform sampler2D splat_map;

uniform vec2 uv_scale = vec2(1.0);

const float level_1 = 25.0;

uniform vec4 water_tint = vec4(0.0, 0.4, 1.0, 1.0);

uniform float water_scale_1 = 0.2;
uniform float water_scale_2 = 0.2;

uniform float texture_speed_1 = 0.1;
uniform float texture_speed_2 = 0.08;

uniform float wobbleX = 1.4;
uniform float wobbleY = 1.7;

uniform float time;

void main()
{
    vec4 water_tex_1 = texture(water_layer_1, vec2((fs_in.TexCoords.x * 8) + (time * texture_speed_1), (fs_in.TexCoords.y * 8) + (cos(time) * texture_speed_1)) + cos(time * vec2(wobbleX, wobbleY) + (fs_in.TexCoords.xy * 8) * 10.0) * 0.02 * 1.0);
    vec4 water_tex_2 = texture(water_layer_2, vec2((fs_in.TexCoords.x * 8) + (sin(time) * texture_speed_2), (fs_in.TexCoords.y * 8) + (time * texture_speed_2)) + cos(time * vec2(wobbleX, wobbleY) + (fs_in.TexCoords.xy * 8) * 10.0) * 0.02 * 1.0);

    vec4 lighting = vec4(texture(light_map, fs_in.TexCoords * vec2(1.0, 1.0)).rrr, 1.0);

    vec4 water = water_tint + (water_tex_1 * water_scale_1) + (water_tex_2 * water_scale_2);

    vec4 color_1 = texture(layer_1, fs_in.TexCoords * (uv_scale * 8)) * texture(noise, fs_in.TexCoords * (uv_scale));
    vec4 color_2 = texture(layer_2, fs_in.TexCoords * (uv_scale * 2)) * texture(noise, fs_in.TexCoords * (uv_scale));
    vec4 color_3 = texture(layer_3, fs_in.TexCoords * (uv_scale * 2)) * texture(noise, fs_in.TexCoords * (uv_scale));
    vec4 color_4 = texture(layer_4, fs_in.TexCoords * (uv_scale * 5)) * texture(noise, fs_in.TexCoords * (uv_scale));

    vec4 splat_weights = texture(splat_map, fs_in.TexCoords);
    vec4 color = splat_weights.r * color_1 +
                  splat_weights.g * color_2 +
                  splat_weights.b * color_3 +
                  (1.0 - splat_weights.r - splat_weights.g - splat_weights.b) * color_4;

    if (Height < level_1)
    {
        FragColor = (water + 0.01) + color;
    }
    else
    {
        FragColor = color * lighting;
    }
}
