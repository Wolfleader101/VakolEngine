#version 460 core
out vec4 FragColor;

struct Light
{
    vec3 position;
    vec3 direction;
};

layout (std140, binding = 2) uniform LightInfo
{
    float constant;
    float linear;
    float quadratic;

    float cut_off;
    float outer_cut_off;
};

in VS_OUT 
{
    vec3 FragPos;
    vec2 TexCoords;
} fs_in;

in float Height;

uniform Light light;

uniform sampler2D light_map;

uniform sampler2D layer_1;
uniform sampler2D layer_2;
uniform sampler2D layer_3;
uniform sampler2D layer_4;
uniform sampler2D layer_5;
uniform sampler2D layer_6;

uniform vec3 VIEW_POS;

uniform vec2 uv_scale = vec2(1.0);

const float level_1 = 20.0;
const float level_2 = 30.0;
const float level_3 = 40.0;
const float level_4 = 60.0;
const float level_5 = 75.0;
const float level_6 = 100.0;

void main()
{
    vec4 lighting = vec4(texture(light_map, fs_in.TexCoords * vec2(1.0, 0.5)).rrr, 1.0);

	vec4 color_1 = texture(layer_1, fs_in.TexCoords * (uv_scale * 5)) * vec4(vec3(0.9), 1.0);
	vec4 color_2 = texture(layer_2, fs_in.TexCoords * (uv_scale * 5));
	vec4 color_3 = texture(layer_3, fs_in.TexCoords * (uv_scale * 1));
	vec4 color_4 = texture(layer_4, fs_in.TexCoords * (uv_scale * 2));
	vec4 color_5 = texture(layer_5, fs_in.TexCoords * (uv_scale * 4));
	vec4 color_6 = texture(layer_6, fs_in.TexCoords * (uv_scale * 1.5));

	vec4 result = vec4(vec3(0.0), 1.0);

    if (Height < level_2)
    {
        float delta = level_2 - level_1;
        float factor = (Height - level_1) / delta;

        result = mix(color_1, color_2, factor);
    }
    else if (Height < level_3)
    {
        float delta = level_3 - level_2;
        float factor = (Height - level_2) / delta;

        result = mix(color_2, color_3, factor);
    }
    else if (Height < level_4)
    {
        float delta = level_4 - level_3;
        float factor = (Height - level_3) / delta;

        result = mix(color_3, color_4, factor);
    }
    else if (Height < level_5)
    {
        float delta = level_5 - level_4;
        float factor = (Height - level_4) / delta;

        result = mix(color_4, color_5, factor);
    }
    else
    {
        float delta = level_6 - level_5;
        float factor = (Height - level_5) / delta;
        
        result = mix(color_5, color_6, factor);
    }
		
	FragColor = result * lighting;
}