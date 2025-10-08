#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
// Per-instance attributes
layout(location = 3) in mat4 a_InstanceMatrix;

layout(std140, binding = 0) uniform Camera
{
    mat4 u_ViewProjection;
} u_Camera;

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
    gl_Position = u_Camera.u_ViewProjection * a_InstanceMatrix * vec4(a_Position, 1.0f);
    Output.TexCoord = vec2(a_TexCoord.x, a_TexCoord.y);
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

layout (binding = 0) uniform sampler2D u_DiffuseTexture;

void main()
{

    o_Color = texture(u_DiffuseTexture, Input.TexCoord);
}
