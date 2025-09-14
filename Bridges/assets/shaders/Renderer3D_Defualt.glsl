#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
//layout(location = 1) in vec4 a_Color;

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
	vec2 TexCoord;

};

layout (location = 0) out VertexOutput Output;
//layout (location = 2) out flat float v_TexIndex;


void main()
{
   // Output.a_Color = a_Color
    gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0f);
    Output.Color = u_objectColor;
    Output.TexCoord = vec2(a_TexCoord.x, 1.0 - a_TexCoord.y);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 o_Color;

struct VertexOutput
{
	vec3 Color;
	vec2 TexCoord;
};

layout (location = 0) in VertexOutput Input;


struct PointLight
{
	vec3 Position;
	vec3 Color;
};

layout(std140, binding = 3) uniform SceneData
{
    vec3 u_CameraPosition;
    int u_NumPointLights;
    PointLight u_PointLights[4];
};


void main()
{

    o_Color = vec4(u_PointLights[0].Color * Input.Color, 1.0 );

    //FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}