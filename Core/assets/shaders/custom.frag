#version 460 core

struct Material 
{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emissive;

    float     shininess;
};

struct Light 
{
    vec3 position;
    vec3 direction; 

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;

uniform Material material;
uniform Light light;

uniform float time;
uniform int option = 0;

vec3 get_light_dir()
{
    if (option == 0)
        return normalize(-light.direction);
    else if (option == 1)
        return normalize(light.position - FragPos);

    return vec3(0.0);
}

vec3 point_light(in vec3 ambient, in vec3 diffuse, in vec3 specular, vec3 dir)
{
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

void main()
{
    /*Ambient */
    vec3 ambient = light.ambient * texture(material.specular, TexCoords).rgb;   /*use specular texture */
    
    /*Diffuse */
    vec3 norm = normalize(Normal);
    vec3 lightDir = get_light_dir();
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.specular, TexCoords).rgb;   /*use specular texture */
    
    /*Specular */
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;    /*use specular texture */
    
    /*Emission */
    vec3 emission = vec3(0.0);
    if (texture(material.specular, TexCoords).r == 0.0)   /*rough check for blackbox inside spec texture */
    {
        /*apply emission texture */
        emission = texture(material.emissive, TexCoords).rgb;
        
        /*some extra fun stuff with "time uniform" */
        emission = texture(material.emissive, TexCoords + vec2(0.0,time)).rgb;   /*moving */
        emission = emission * (sin(time) * 0.5 + 0.5) * 2.0;                     /*fading */
    }
    
    /*output */
    vec3 result = vec3(0.0);

    if (option == 0)
        result = ambient + diffuse + specular + emission;
    else if (option == 1)
        result = point_light(ambient, diffuse, specular, lightDir);

    FragColor = vec4(result, 1.0);
}