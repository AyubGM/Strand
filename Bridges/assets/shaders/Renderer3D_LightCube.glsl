#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;

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

void main()
{
    gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 o_Color;

void main()
{
    o_Color = vec4(1.0); // set all 4 vector values to 1.0
}