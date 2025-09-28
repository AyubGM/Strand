#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

layout (location = 0) out vec3 TexCoords;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
} u_Camera;


void main()
{
    TexCoords = a_Position;
    vec4 pos = u_Camera.u_ViewProjection * vec4(a_Position, 0.0);
    gl_Position = pos.xyww;
}  

#type fragment
#version 450 core

layout(location = 0) out vec4 o_Color;

layout (location = 0) in vec3 TexCoords;

layout (binding = 0) uniform samplerCube skybox;

void main()
{    
    o_Color = texture(skybox, TexCoords);
}