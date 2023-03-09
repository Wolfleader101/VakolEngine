#version 330

uniform mat4 projectionMatrix; //The projection matrix for the engine
uniform mat4 viewMatrix; //The view matrix for the engine

in vec4 a_position;

out VertexData //The vertex data to be sent to the Fragment shader
{
    vec2 v_texcoord;
} outData;

void main(void)
{
    outData.v_texcoord.x = a_position.x;
    outData.v_texcoord.y = a_position.y;
    
    gl_Position = projectionMatrix * viewMatrix * a_position;
}