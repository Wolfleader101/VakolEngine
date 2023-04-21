#version 460 core
out vec4 FragColor;

struct Material
{
    sampler2D diffuse_map;
    
    float shininess;
};

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

in VS_OUT {
    vec4 FragCoords;
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

const int DIRECTIONAL_LIGHT = 0;
const int POINT_LIGHT = 1;
const int SPOT_LIGHT = 2;

uniform Material material;
uniform Light light;

uniform vec3 VIEW_POS;

uniform vec3 tint = vec3(0.8);

uniform float FOG_DENSITY = 0.1;
uniform vec4 FOG_COLOR = vec4(1.0, 1.0, 1.0, 0.0);

uniform int OPTION = 0;
uniform bool enable_textures = false;
uniform bool enable_fog = false;

vec3 get_light_direction()
{
    if (OPTION == DIRECTIONAL_LIGHT)
        return normalize(-light.direction);
    else if (OPTION == POINT_LIGHT || OPTION == SPOT_LIGHT)
        return normalize(light.position - fs_in.FragPos);
}

float calculate_fog(float fogCoords)
{
    float result = 0.0;

    // result = exp(-density * fogCoords); // Equation 1

    result = exp(-pow(FOG_DENSITY * fogCoords, 2.0)); // Equation 2

    return 1.0 - clamp(result, 0.0, 1.0);
}

vec4 BlinnPhong(vec3 normal, vec4 color)
{
    // ambient
    vec4 ambient = 0.1 * color;

    // diffuse
    vec3 lightDir = get_light_direction();

    float diff = max(dot(lightDir, normal), 0.0);
    vec4 diffuse = diff * color;

    // specular
    vec3 viewDir = normalize(VIEW_POS - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;

    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec4 specular = vec4(vec3(0.6), 1.0) * spec; // assuming bright white light color

    if (OPTION == SPOT_LIGHT)
    {
        float theta = dot(lightDir, normalize(-light.direction));
        float epsilon = cut_off - outer_cut_off;
        float intensity = clamp((theta - outer_cut_off) / epsilon, 0.0, 1.0);

        diffuse *= intensity;
        specular *= intensity;
    }

    // simple attenuation
    if (OPTION != DIRECTIONAL_LIGHT)
    {
        float distance = length(light.position - fs_in.FragPos);
        float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;
    }

    return ambient + diffuse + specular;
}

void main()
{           
    vec4 color = vec4(vec3(0.0), 1.0);

    if (enable_textures)
        color = texture(material.diffuse_map, fs_in.TexCoords);
    else 
        color = vec4(tint, 1.0);

    vec4 lighting = vec4(vec3(0.0), 1.0);

    lighting += BlinnPhong(normalize(fs_in.Normal), color);
    color *= lighting;

    color = pow(color, vec4(vec3(1.0 / 2.2), 1.0));

    if (enable_fog)
    {
        float fogCoords = abs(fs_in.FragCoords.z / fs_in.FragCoords.w);
        FragColor = mix(color, FOG_COLOR, calculate_fog(fogCoords));
    }
    else
        FragColor = color;
}