#version 460 core

out vec4 FragColor;

struct Material
{
    sampler2D diffuse_map;
    sampler2D normal_map;

    float shininess;
};

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

uniform Material material;

uniform bool gamma;

uniform vec3 lightPos;
uniform vec3 viewPos;

vec3 BlinnPhong(in vec3 color)
{
    // obtain normal from normal map in range [0,1]
    vec3 normal = texture(material.normal_map, fs_in.TexCoords).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
   
    // diffuse
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    
    // specular
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    
    float spec = 0.0;
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec3 specular = spec * vec3(1.0);

    // simple attenuation
    float max_distance = 1.5;
    float distance = length(lightPos - fs_in.TangentFragPos);
    float attentuation = 1.0 / (gamma ? distance * distance : distance);

    diffuse *= attentuation;
    specular *= attentuation;

    return diffuse + specular;
}

void main()
{
    // get diffuse color
    vec3 color = texture(material.diffuse_map, fs_in.TexCoords).rgb;

    vec3 lighting = vec3(0.0);

    lighting += BlinnPhong(color);
    color *= lighting;

    if (gamma)
        color = pow(color, vec3(1.0 / 2.2));

    FragColor = vec4(color, 1.0);
}