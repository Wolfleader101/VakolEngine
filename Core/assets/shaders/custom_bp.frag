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
};

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

const int DIRECTIONAL_LIGHT = 0;
const int POINT_LIGHT = 1;

uniform Material material;
uniform Light light;

uniform vec3 VIEW_POS;
uniform vec3 tint = vec3(0.8);

uniform int option = 0;

vec3 get_light_direction()
{
    if (option == DIRECTIONAL_LIGHT)
        return normalize(-light.direction);
    else if (option == POINT_LIGHT)
        return normalize(light.position - fs_in.FragPos);
}

vec3 BlinnPhong(vec3 normal, vec3 color)
{
    // ambient
    vec3 ambient = 0.05 * color;

    // diffuse
    vec3 lightDir = get_light_direction();

    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;

    // specular
    vec3 viewDir = normalize(VIEW_POS - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;

    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec3 specular = vec3(0.6) * spec; // assuming bright white light color

    // simple attenuation
    if (option != DIRECTIONAL_LIGHT)
    {
        float distance = length(light.position - fs_in.FragPos);
        float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

        diffuse *= attenuation;
        specular *= attenuation;
    }

    return diffuse + specular;
}

void main()
{           
    vec3 color = tint;//texture(material.diffuse_map, fs_in.TexCoords).rgb;

    vec3 lighting = vec3(0.0);

    lighting += BlinnPhong(normalize(fs_in.Normal), color);
    color *= lighting;

    color = pow(color, vec3(1.0 / 2.2));
    
    FragColor = vec4(color, 1.0);
}