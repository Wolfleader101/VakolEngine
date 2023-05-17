#version 460 core
out vec4 FragColor;

in VS_OUT
{
    vec3  fragPos;
	vec3  normal;
    vec2  uv;
    mat3  TBN;
} fs_in;

struct Material
{
    sampler2D diffuse_map;
    sampler2D specular_map;
    sampler2D normal_map;
    sampler2D emission_map;
    
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

uniform Material material;
uniform Light light;

uniform vec3 VIEW_POS;
uniform vec3 tint = vec3(1.0);

vec4 BlinnPhong(const vec3 normal, const vec4 color)
{
    vec4 ambient = 0.1 * color;

    vec3 light_dir = normalize(light.position - fs_in.fragPos);
    float diff = max(dot(light_dir, normal), 0.0);
    vec4 diffuse = diff * color;

    vec3 view_dir = normalize(VIEW_POS - fs_in.fragPos);
    vec3 reflect_dir = reflect(-light_dir, normal);
    
    vec3 halfway_dir = normalize(light_dir + view_dir);
    float spec = pow(max(dot(normal, halfway_dir), 0.0), material.shininess);
    vec4 specular = vec4(vec3(0.6), 1.0) * spec;

    float distance = length(light.position - fs_in.fragPos);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

void main()
{
    vec4 color = vec4(vec3(0.0), 1.0);
    vec3 normal = texture(material.normal_map, fs_in.uv).rgb;

    if (normal.r <= 0.0 && normal.g <= 0.0 && normal.b <= 0.0)
        normal = normalize(fs_in.normal);
    else
    {
        normal = normal * 2.0 - 1.0; // range between [0, 1]
        normal = normalize(fs_in.TBN * normal);
    }

    color = texture(material.diffuse_map, fs_in.uv);

    if (color.r <= 0.0 && color.b <= 0.0 && color.b <= 0.0)
        color = vec4(tint, 1.0);

    vec4 lighting = vec4(vec3(0.0), 1.0);

    lighting += BlinnPhong(normal, color);
    color *= lighting;

    color = pow(color, vec4(vec3(1.0 / 2.2), 1.0));

    FragColor = color;
}