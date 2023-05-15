#version 460 core
out vec4 FragColor;

in VS_OUT
{
    vec3  fragPos;
	vec3  normal;
    vec2  uv;
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

uniform Material material;
uniform Light light;

uniform vec3 VIEW_POS;

vec4 BlinnPhong(vec3 normal, vec4 color)
{
    // ambient
    vec4 ambient = 0.1 * color;

    // diffuse
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(lightDir, normal), 0.0);
    vec4 diffuse = diff * color;

    // specular
    vec3 viewDir = normalize(VIEW_POS - fs_in.fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;

    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec4 specular = vec4(vec3(0.6), 1.0) * spec; // assuming bright white light color

    return ambient + diffuse + specular;
}

void main()
{
    vec4 color = texture(material.diffuse_map, fs_in.uv);

//    vec4 lighting = vec4(vec3(0.0), 1.0);
//
//    lighting += BlinnPhong(normalize(fs_in.normal), color);
//    color *= lighting;
//
//    color = pow(color, vec4(vec3(1.0 / 2.2), 1.0));

    FragColor = color;
}