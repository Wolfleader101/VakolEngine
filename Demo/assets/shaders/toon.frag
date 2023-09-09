#version 460 core
out vec4 FragColor;

struct Material
{
    vec3 ambient_color;
    vec3 diffuse_color;
    vec3 specular_color;
    vec3 emissive_color;

    float shininess;
    float shininess_strength;
    float opacity;

    bool use_lighting;
    bool use_textures;

    sampler2D diffuse_map;
    sampler2D specular_map;
    sampler2D ambient_map;
    sampler2D emissive_map;
    sampler2D height_map;
    sampler2D normal_map;
};

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 Normal;
} fs_in;

uniform Material material;

const int levels = 3;
const float scaleFactor = 1.0 / 3.0;

uniform vec3 LIGHT_POSITION = vec3(0.0, 10.0, 0.0);

vec4 ToonShading(const vec3 normal, const vec4 color)
{
    vec4 ambient = vec4(0.5);

    vec3 lightDir = normalize(LIGHT_POSITION - fs_in.FragPos);
    float diff = max(0.0, dot(lightDir, normal));
    
    vec4 diffuse = color * (ceil(diff * levels) * scaleFactor);

    // light strength * (ambient + diffuse)
    return 0.4 * (ambient + diffuse);
}

void main()
{    
    vec4 color = texture(material.diffuse_map, fs_in.TexCoords);

    if ((color.r >= 0.99 && color.g >= 0.99 && color.b >= 0.99) || !material.use_textures)
        color = vec4(material.diffuse_color, 1.0);

    vec3 normal = fs_in.Normal;

    if (material.use_lighting)
    {
        vec4 result = ToonShading(normal, color);
        FragColor = result;
    }
    else
        FragColor = color;
}