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

    float cut_off;
    float outer_cut_off;
};

const int DIRECTIONAL_LIGHT = 0;
const int POINT_LIGHT = 1;
const int SPOT_LIGHT = 2;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;
uniform vec2 viewPort;

uniform Material material;
uniform Light light;

uniform float time;
uniform int option = 0;

vec3 get_light_dir()
{
    if (option == DIRECTIONAL_LIGHT)
        return normalize(-light.direction);
    else if (option == POINT_LIGHT || option == SPOT_LIGHT)
        return normalize(light.position - FragPos);

    return vec3(0.0);
}

void point_light(inout vec3 ambient, inout vec3 diffuse, inout vec3 specular, inout vec3 emission, bool spot)
{
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 

    if (!spot)
        ambient *= attenuation;

    diffuse *= attenuation;
    specular *= attenuation;
    emission *= attenuation;
}

void spot_light(inout vec3 ambient, inout vec3 diffuse, inout vec3 specular, inout vec3 emission, vec3 dir)
{
    float theta = dot(dir, normalize(-light.direction));
    float epsilon = light.cut_off - light.outer_cut_off;
    float intensity = smoothstep(0.0, 1.0, (theta - light.outer_cut_off) / epsilon);

    diffuse *= intensity;
    specular *= intensity;
    emission *= intensity;

    point_light(ambient, diffuse, specular, emission, true);
}

void lighting(inout vec3 ambient, inout vec3 diffuse, inout vec3 specular, inout vec3 emission, out vec3 lightDir)
{
    /*Ambient */
    ambient = light.ambient * texture(material.specular, TexCoords).rgb;   /*use specular texture */
    
    /*Diffuse */
    vec3 norm = normalize(Normal);
    lightDir = get_light_dir();
    float diff = max(dot(norm, lightDir), 0.0);
    diffuse = light.diffuse * diff * texture(material.specular, TexCoords).rgb;   /*use specular texture */
    
    /*Specular */
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    specular = light.specular * spec * texture(material.specular, TexCoords).rgb;    /*use specular texture */
    
    /*Emission */
    if (texture(material.specular, TexCoords).r == 0.0)   /*rough check for blackbox inside spec texture */
    {
        /*apply emission texture */
        emission = texture(material.emissive, TexCoords).rgb;
        
        /*some extra fun stuff with "time uniform" */
        emission = texture(material.emissive, TexCoords + vec2(0.0,time)).rgb;   /*moving */
        emission = emission * (sin(time) * 0.5 + 0.5) * 2.0;                     /*fading */
    }
}

void main()
{
    vec3 ambient = vec3(0.0);
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);
    vec3 emission = vec3(0.0);
    vec3 lightDir = vec3(0.0);

    lighting(ambient, diffuse, specular, emission, lightDir);

    vec3 result = vec3(0.0);

    if (option == DIRECTIONAL_LIGHT)
        result = ambient + diffuse + specular + emission;
    else if (option == POINT_LIGHT)
    {
        point_light(ambient, diffuse, specular, emission, false);
        result = ambient + diffuse + specular + emission;
    }
    else if (option == SPOT_LIGHT)
    {
        spot_light(ambient, diffuse, specular, emission, lightDir);
        result = ambient + diffuse + specular + emission;
    }

    FragColor = vec4(result, 1.0);    
}