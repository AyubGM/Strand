#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;


layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
} u_Camera;

layout(std140, binding = 1) uniform ObjectData
{
    mat4 u_Model;
    mat4 u_NormalMatrix;
    vec3 u_objectColor;
} u_ObjectData;

struct VertexOutput
{
	vec3 Color;
    vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
};

layout (location = 0) out VertexOutput Output;

void main()
{
    Output.TexCoord = vec2(a_TexCoord.x, 1.0 - a_TexCoord.y);   
    gl_Position = u_Camera.u_ViewProjection * u_ObjectData.u_Model * vec4(a_Position, 1.0f);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 o_Color;


struct VertexOutput
{
	vec3 Color;
    vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
};

layout (location = 0) in VertexOutput Input;

layout (binding = 1) uniform sampler2D u_ModelDiffuseTexture;

void main()
{    
    o_Color = vec4(vec3(texture(u_ModelDiffuseTexture, Input.TexCoord)), 1.0);
}