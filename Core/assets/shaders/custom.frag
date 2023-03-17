#version 460 core

struct Material
{
    sampler2D texture_diffuse;
    sampler2D texture_specular;

    float shininess;
};

struct Light
{
    vec3 position;
    vec3 color;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;

uniform Material material;
uniform Light light;

void main()
{
    // Ambient
    //vec3 ambient = light.ambient * light.color;
    vec3 ambient = light.ambient * texture(material.texture_diffuse, TexCoords).rgb;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    //vec3 diffuse = light.diffuse * diff * light.color;
    vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse, TexCoords).rgb;

    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //vec3 specular = light.specular * spec * light.color;
    vec3 specular = light.specular * spec * texture(material.texture_specular, TexCoords).rgb;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}