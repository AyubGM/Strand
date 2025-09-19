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
    vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;

};

layout (location = 0) out VertexOutput Output;
//layout (location = 2) out flat float v_TexIndex;


void main()
{
   // Output.a_Color = a_Color
    gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0f);
    Output.Color = u_objectColor;
    Output.FragPos = vec3(u_Model * vec4(a_Position, 1.0f));
    Output.Normal = normalize(mat3(u_NormalMatrix) * a_Normal);
    Output.TexCoord = vec2(a_TexCoord.x, 1.0 - a_TexCoord.y);
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

	vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

struct Material {
    float shininess;
}; 

layout(std140, binding = 2) uniform MaterialData
{
    Material u_Material;
};

layout (binding = 0) uniform sampler2D u_DiffuseTexture;
layout (binding = 1) uniform sampler2D u_SpecularTexture;

layout(std140, binding = 3) uniform SceneData
{
    vec3 u_CameraPosition;
    int u_NumPointLights;
    PointLight u_PointLights[4];
};


void main()
{
    // ambient
    float ambientStrength = 0.2;
    vec3 ambient = u_PointLights[0].ambient.rgb *  vec3(texture(u_DiffuseTexture, Input.TexCoord));

    // diffuse 
    vec3 norm = normalize(Input.Normal);
    vec3 lightDir = normalize(u_PointLights[0].Position.xyz - Input.FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse =  u_PointLights[0].diffuse.rgb * diff * vec3(texture(u_DiffuseTexture, Input.TexCoord));

     // specular
    vec3 viewDir = normalize(u_CameraPosition - Input.FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    vec3 specular = u_PointLights[0].specular.rgb * spec * vec3(texture(u_SpecularTexture, Input.TexCoord));

    vec3 result = ambient + diffuse + specular;
    o_Color = vec4(  result , 1.0 );

    //FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}