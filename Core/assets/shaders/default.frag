#version 460 core

out vec4 FragColor;

const uint DIRECTIONAL_LIGHT = 0;
const uint POINT_LIGHT = 1;
const uint SPOT_LIGHT = 2;

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
    uint TYPE;

    vec3 DIRECTION;

    float ATTENUATION_CONSTANT;
    float ATTENUATION_LINEAR;
    float ATTENUATION_QUADRATIC;

    float ATTENUATION_CUTOFF;
    float ATTENUATION_OUTER_CUTOFF;
};

uniform Material material;
uniform Light light;

uniform vec2 UV_OFFSET = vec2(0.0);

uniform float AMBIENT_STRENGTH = 0.2;
uniform float SPECULAR_STRENGTH = 0.3;

vec4 BlinnPhong(const vec3 normal, const vec4 color)
{
    vec4 ambient = AMBIENT_STRENGTH * color;

    vec3 lightDir = vec3(0.0);

    if (light.TYPE == DIRECTIONAL_LIGHT)
        lightDir = normalize(radians(-light.DIRECTION));
    else if (light.TYPE == POINT_LIGHT || light.TYPE == SPOT_LIGHT)
        lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);

    float diff = max(dot(normal, lightDir), 0.0);
    vec4 diffuse = diff * color;

    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec4 specular = vec4(vec3(SPECULAR_STRENGTH), 1.0) * spec;

    if (light.TYPE == SPOT_LIGHT)
    {
        float theta = dot(lightDir, normalize(radians(-light.DIRECTION)));
        float epsilon = radians(light.ATTENUATION_CUTOFF) - radians(light.ATTENUATION_OUTER_CUTOFF);
        float intensity = clamp((theta - radians(light.ATTENUATION_OUTER_CUTOFF)) / epsilon, 0.0, 1.0);
        
        diffuse *= intensity;
        specular *= intensity;
    }

    if (light.TYPE == POINT_LIGHT || light.TYPE == SPOT_LIGHT)
    {
        float distance = length(light.DIRECTION - fs_in.TangentFragPos);
        float attenuation = 1.0 / (light.ATTENUATION_CONSTANT + light.ATTENUATION_LINEAR * distance + light.ATTENUATION_QUADRATIC * (distance * distance));

        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;
    }

    return ambient + diffuse + specular;
}

void main()
{
    vec4 color = texture(material.diffuse_map, fs_in.TexCoords + UV_OFFSET);

    if ((color.r >= 0.99 && color.g >= 0.99 && color.b >= 0.99) || !material.use_textures)
        color = material.diffuse_color;

    // Kiki eye hack
    if (material.opacity < 1.0)
        color = texture(material.diffuse_map, fs_in.TexCoords + UV_OFFSET);

    vec3 normal = vec3(0.0);

    if (normal.r >= 0.99 && normal.g >= 0.99 && normal.b >= 0.99)
        normal = normalize(fs_in.Normal);
    else
    {
        normal = texture(material.normal_map, fs_in.TexCoords).rgb;
        normal = normalize(normal * 2.0 - 1.0); // this normal is now in tangent space in range [-1, 1]
    }

    if (material.use_lighting)
    {
        vec4 result = BlinnPhong(normal, color);
	    FragColor = result;
    }
    else
        FragColor = color;
}