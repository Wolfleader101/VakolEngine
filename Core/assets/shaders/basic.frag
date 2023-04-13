#version 460 core
out vec4 FragColor;

in vec2 uv;

void main()
{
    // if (enableTexture)
    //     FragColor = texture(inputTexture0, uv * uvScale0) * rgba;
    // else
    //     FragColor = rgba;

    FragColor = vec4(1.0);
}