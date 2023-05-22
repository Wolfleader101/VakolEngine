#version 460 core
out vec4 FragColor;

in VS_OUT 
{
    vec3 FragPos;
    vec2 TexCoords;
} fs_in;

in float Height;

uniform sampler2D light_map;

uniform sampler2D layer_1;
uniform sampler2D layer_2;
uniform sampler2D layer_3;
uniform sampler2D layer_4;
uniform sampler2D layer_5;
uniform sampler2D layer_6;
uniform sampler2D layer_7;
uniform sampler2D layer_8;
uniform sampler2D layer_9;
uniform sampler2D layer_10;

uniform vec2 uv_scale = vec2(1.0);

const float level_1 = 5.0;
const float level_2 = 15.0;
const float level_3 = 45.0;
const float level_4 = 75.0;
const float level_5 = 95.0;
const float level_6 = 115.0;
const float level_7 = 130.0;
const float level_8 = 155.0;
const float level_9 = 180.0;
const float level_10 = 255.0;

void main()
{
    vec4 lighting = vec4(texture(light_map, fs_in.TexCoords * vec2(1.0, 1.0)).rrr, 1.0);

	vec4 color_1 = texture(layer_1, fs_in.TexCoords * (uv_scale * 6));
	vec4 color_2 = texture(layer_2, fs_in.TexCoords * (uv_scale * 2));
	vec4 color_3 = texture(layer_3, fs_in.TexCoords * (uv_scale * 4));
	vec4 color_4 = texture(layer_4, fs_in.TexCoords * (uv_scale * 4));
	vec4 color_5 = texture(layer_5, fs_in.TexCoords * (uv_scale * 4));
	vec4 color_6 = texture(layer_6, fs_in.TexCoords * (uv_scale * 15));
	vec4 color_7 = texture(layer_7, fs_in.TexCoords * (uv_scale * 4));
	vec4 color_8 = texture(layer_8, fs_in.TexCoords * (uv_scale * 15));
	vec4 color_9 = texture(layer_9, fs_in.TexCoords * (uv_scale * 4));
	vec4 color_10 = texture(layer_10, fs_in.TexCoords * (uv_scale * 1.5));

	vec4 result = vec4(vec3(0.0), 1.0);

    if (Height < level_1)
    {
        result = mix(vec4(vec3(0.0), 1.0), color_1, 0.99);
    }    
    else if (Height < level_2)
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
    else if (Height < level_6)
    {
        float delta = level_6 - level_5;
        float factor = (Height - level_5) / delta;
        
        result = mix(color_5, color_6, factor);
    }
    else if (Height < level_7)
    {
        float delta = level_7 - level_6;
        float factor = (Height - level_6) / delta;
        
        result = mix(color_6, color_7, factor);
    }
    else if (Height < level_8)
    {
        float delta = level_8 - level_7;
        float factor = (Height - level_7) / delta;
        
        result = mix(color_7, color_8, factor);
    }
    else if (Height < level_9)
    {
        float delta = level_9 - level_8;
        float factor = (Height - level_8) / delta;
        
        result = mix(color_8, color_9, factor);
    }
    else if (Height < level_10)
    {
        float delta = level_10 - level_9;
        float factor = (Height - level_9) / delta;
        
        result = mix(color_9, color_10, factor);
    }
		
	FragColor = result * lighting;
}