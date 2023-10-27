#version 460 core

out vec4 FragColor;

const uint DIRECTIONAL_LIGHT = 1;
const uint POINT_LIGHT = 2;
const uint SPOT_LIGHT = 3;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 Normal;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

struct Material
{
    vec4 ambient_color;
    vec4 diffuse_color;
    vec4 specular_color;
    vec4 emissive_color;

    float shininess;
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

struct Light
{
    uint type;

    float LIGHT_ATTENUATION_CONSTANT;
    float LIGHT_ATTENUATION_LINEAR;
    float LIGHT_ATTENUATION_QUADRATIC;

    float LIGHT_ATTENUATION_CUTOFF;
    float LIGHT_ATTENUATION_OUTER_CUTOFF;

    vec4 direction;
    vec4 position;
};

uniform Material material;

uniform vec3 LIGHT_DIRECTION;

uniform vec2 UV_OFFSET = vec2(0.0);

uniform float AMBIENT_STRENGTH = 0.2;
uniform float SPECULAR_STRENGTH = 0.3;

vec4 BlinnPhong(const vec3 normal, const vec4 color)
{
    vec4 ambient = AMBIENT_STRENGTH * color;

    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec4 diffuse = diff * color;

    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec4 specular = vec4(vec3(SPECULAR_STRENGTH), 1.0) * spec;

    return ambient + diffuse + specular;
}

void main()
{
    vec4 color = texture(material.diffuse_map, fs_in.TexCoords + UV_OFFSET);

    if (!material.use_textures)
        color = material.diffuse_color;

    vec3 normal = texture(material.normal_map, fs_in.TexCoords).rgb;

    if (normal.r >= 0.99 && normal.g >= 0.99 && normal.b >= 0.99)
        normal = normalize(fs_in.Normal);
    else
        normal = normalize(normal * 2.0 - 1.0); // this normal is now in tangent space in range [-1, 1]

    if (material.use_lighting)
    {
        vec4 result = BlinnPhong(normal, color);
	    FragColor = result;
    }
    else
        FragColor = color;
}