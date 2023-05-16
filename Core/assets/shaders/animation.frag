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

uniform Material material;
uniform Light light;

uniform vec3 VIEW_POS;

void main()
{
    vec3 light_color = vec3(1.0);

    float ambient_K = 0.6;
    float specular_K = 0.6;

    vec3 ambient = ambient_K * light_color;

    vec3 N = vec3(texture(material.normal_map, fs_in.uv));
    N = N * 2.0 - 1.0;
    N = normalize(fs_in.TBN * N);

    vec3 light_dir = normalize(light.position - fs_in.fragPos);
    float diff = max(dot(N, light_dir), 0.0);
    vec3 diffuse = diff * light_color;

    vec3 view_dir = normalize(VIEW_POS - fs_in.fragPos);
    vec3 reflect_dir = reflect(-light_dir, N);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
    vec3 specular = material.shininess * spec * light_color;

    vec3 color = vec3(texture(material.diffuse_map, fs_in.uv));
    vec3 result = (ambient + diffuse + specular) * color;

    FragColor = vec4(result, 1.0);
}