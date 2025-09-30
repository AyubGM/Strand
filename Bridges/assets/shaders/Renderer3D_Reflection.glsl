#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;


layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
};

layout(std140, binding = 1) uniform ObjectData
{
    mat4 u_Model;
    mat4 u_NormalMatrix;
    vec3 u_objectColor;
};

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
    Output.Normal = normalize(mat3(u_NormalMatrix) * a_Normal);
    Output.FragPos = vec3(u_Model * vec4(a_Position, 1.0f));
    gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0f);
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



struct PointLight
{
	vec4 Position;

	vec4 Ambient;
    vec4 Diffuse;
    vec4 Specular;

    float Constant;
    float Linear;
    float Quadratic;
};

struct DirectLight {
    vec4 Direction;
  
    vec4 Ambient;
    vec4 Diffuse;
    vec4 Specular;
};

struct Spotlight {
    vec4 Position;  
    vec4 Direction;

    vec4 Ambient;
    vec4 Diffuse;
    vec4 Specular;

    float CutOff;
    float OuterCutOff;
	
    float Constant;
    float Linear;
    float Quadratic;
};

layout(std140, binding = 3) uniform SceneData
{
    vec3 u_CameraPosition;
    int u_NumPointLights;
    PointLight u_PointLights[4];
    DirectLight u_DirectLight;
    Spotlight u_Spotlight;
};

layout (binding = 5) uniform samplerCube skybox;

void main()
{    
    vec3 I = normalize(Input.FragPos - u_CameraPosition);
    vec3 R = reflect(I, normalize(Input.Normal));
    o_Color = vec4(texture(skybox, R).rgb, 1.0);
}