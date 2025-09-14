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

};

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;

};

layout (location = 0) out VertexOutput Output;
//layout (location = 2) out flat float v_TexIndex;


void main()
{
   // Output.a_Color = a_Color
    gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0f);
    Output.TexCoord = vec2(a_TexCoord.x, 1.0 - a_TexCoord.y);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 o_Color;

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
};

layout (location = 0) in VertexOutput Input;


void main()
{
    //vec4 texColor = Input.Color;
    vec4 texColor = vec4(1.0,1.0,1.0,1.0);
    o_Color = texColor;

    //FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}