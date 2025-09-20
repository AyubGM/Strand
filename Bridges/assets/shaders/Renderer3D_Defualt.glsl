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
    PointLight u_PointLights[u_NumPointLights];
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
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(u_PointLights[i], norm, Input.FragPos, viewDir); 

    // phase 3: Spot light
    result += CalcSpotLight(u_Spotlight, norm, Input.FragPos, viewDir);  

    o_Color = vec4(  result , 1.0 );

    //FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}


vec3 CalcDirLight(DirectLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.Direction.xyz);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.Shininess);
    // combine results
    vec3 ambient  = light.Ambient.rgb  * vec3(texture(u_DiffuseTexture, Input.TexCoord));
    vec3 diffuse  = light.Diffuse.rgb  * diff * vec3(texture(u_DiffuseTexture, Input.TexCoord));
    vec3 specular = light.Specular.rgb * spec * vec3(texture(u_SpecularTexture, Input.TexCoord));
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir =  normalize(light.Position.xyz - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0),  u_Material.Shininess);

    // attenuation
    float distance    = length(light.Position.xyz - fragPos);
    float attenuation = 1.0 / (light.Constant + light.Linear * distance + 
  			     light.Quadratic * (distance * distance));    
    // combine results
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
    // ambient
    vec3 ambient  = light.Ambient.rgb  * vec3(texture(u_DiffuseTexture, Input.TexCoord));
    
    // diffuse 
    vec3 lightDir = normalize(light.Position.xyz - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse  = light.Diffuse.rgb  * diff * vec3(texture(u_DiffuseTexture, Input.TexCoord));
    
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0),  u_Material.Shininess);
    vec3 specular = light.Specular.rgb * spec *  vec3(texture(u_SpecularTexture, Input.TexCoord));  
    
    // spotlight (soft edges)
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = (light.CutOff - light.OuterCutOff);
    float intensity = clamp((theta - light.OuterCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;
    
    // attenuation
    float distance    = length(light.Position.xyz - fragPos);
    float attenuation = 1.0 / (light.Constant + light.Linear * distance + light.Quadratic * (distance * distance));    
    ambient  *= attenuation; 
    diffuse   *= attenuation;
    specular *= attenuation;   
        
    return (ambient + diffuse + specular);
}