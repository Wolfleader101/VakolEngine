#version 330

uniform mat4 mvp; //The Model View Projection matrix

in vec4 a_position;
in vec3 a_normal;
in vec2 a_texcoord;

out VertexData //The vertex data to be sent to the Fragment shader
{
    vec4 v_position;
    vec3 v_normal;
    vec2 v_texcoord;
} outData;

void main(void)
{
    gl_Position = mvp * a_position; //The position of the model

    outData.v_position = a_position;
    outData.v_normal = a_normal;
    outData.v_texcoord = a_texcoord;
}