#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
// Per-instance attributes
layout(location = 3) in mat4 a_Model;
layout(location = 7) in mat4 a_NormalMatrix;
layout(location = 11) in vec3 a_ObjectColor;

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
    gl_Position = u_Camera.u_ViewProjection * a_Model * vec4(a_Position, 1.0f);
    Output.Color = a_ObjectColor;
    Output.FragPos = vec3(a_Model * vec4(a_Position, 1.0f));
    Output.Normal = normalize(mat3(a_NormalMatrix) * a_Normal);
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

struct Material {
    float Shininess;
} ; 

layout(std140, binding = 2) uniform MaterialData
{
    Material u_MaterialData;
} u_Material;

layout (binding = 0) uniform sampler2D u_DiffuseTexture;
layout (binding = 1) uniform sampler2D u_SpecularTexture;

layout(std140, binding = 3) uniform SceneData
{
    vec3 u_CameraPosition;
    int u_NumPointLights;
    PointLight u_PointLights[4];
    DirectLight u_DirectLight;
    Spotlight u_Spotlight;
};

vec3 CalcDirLight(DirectLight light, vec3 normal, vec3 viewDir);  
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(Spotlight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    // properties
    vec3 norm = normalize(Input.Normal);
    vec3 viewDir = normalize(u_CameraPosition - Input.FragPos);

    // phase 1: Directional lighting
    vec3 result = CalcDirLight(u_DirectLight, norm, viewDir);

    // phase 2: Point lights
    for(int i = 0; i < u_NumPointLights; i++)
        result += CalcPointLight(u_PointLights[i], norm, Input.FragPos, viewDir); 

    // phase 3: Spot light
    result += CalcSpotLight(u_Spotlight, norm, Input.FragPos, viewDir);  

    o_Color = vec4(  result , 1.0 );
}

vec3 CalcDirLight(DirectLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.Direction.xyz);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.u_MaterialData.Shininess);
    vec3 ambient  = light.Ambient.rgb  * vec3(texture(u_DiffuseTexture, Input.TexCoord));
    vec3 diffuse  = light.Diffuse.rgb  * diff * vec3(texture(u_DiffuseTexture, Input.TexCoord));
    vec3 specular = light.Specular.rgb * spec * vec3(texture(u_SpecularTexture, Input.TexCoord));
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir =  normalize(light.Position.xyz - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.u_MaterialData.Shininess);
    float distance    = length(light.Position.xyz - fragPos);
    float attenuation = 1.0 / (light.Constant + light.Linear * distance + 
                     light.Quadratic * (distance * distance));    
    vec3 ambient  = light.Ambient.rgb  * vec3(texture(u_DiffuseTexture, Input.TexCoord));
    vec3 diffuse  = light.Diffuse.rgb  * diff * vec3(texture(u_DiffuseTexture, Input.TexCoord));
    vec3 specular = light.Specular.rgb * spec *  vec3(texture(u_SpecularTexture, Input.TexCoord));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 

vec3 CalcSpotLight(Spotlight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 ambient  = light.Ambient.rgb  * vec3(texture(u_DiffuseTexture, Input.TexCoord));
    vec3 lightDir = normalize(light.Position.xyz - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse  = light.Diffuse.rgb  * diff * vec3(texture(u_DiffuseTexture, Input.TexCoord));
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.u_MaterialData.Shininess);
    vec3 specular = light.Specular.rgb * spec *  vec3(texture(u_SpecularTexture, Input.TexCoord));  
    float theta = dot(lightDir, normalize(-light.Direction.xyz)); 
    float epsilon = (light.CutOff - light.OuterCutOff);
    float intensity = clamp((theta - light.OuterCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;
    float distance    = length(light.Position.xyz - fragPos);
    float attenuation = 1.0 / (light.Constant + light.Linear * distance + light.Quadratic * (distance * distance));    
    ambient  *= attenuation; 
    diffuse   *= attenuation;
    specular *= attenuation;   
    return (ambient + diffuse + specular);
}
