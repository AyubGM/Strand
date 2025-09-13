#type vertex
#version 450 core

// Input vertex attributes from the mesh
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

// Uniform buffer for camera data
layout(std140, binding = 0) uniform CameraData
{
	mat4 u_ViewProjection;
};

// Uniforms for the model's transformation and per-object data
uniform mat4 u_Model;

// Output to the fragment shader
out vec3 v_WorldPosition;
out vec3 v_Normal;
out vec2 v_TexCoord;

void main()
{
	// Calculate the world-space position of the vertex
	v_WorldPosition = vec3(u_Model * vec4(a_Position, 1.0));
	
	// Calculate the normal vector in world space
	v_Normal = mat3(u_Model) * a_Normal;

	// Pass through the texture coordinates
	v_TexCoord = a_TexCoord;
	
	// Final position of the vertex on the screen
	gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

// Input from the vertex shader
in vec3 v_WorldPosition;
in vec3 v_Normal;
in vec2 v_TexCoord;

// Per-object material properties
uniform vec3 u_Albedo;
uniform float u_Metallic;
uniform float u_Roughness;
uniform float u_AO;

// Scene-wide uniforms
uniform vec3 u_CameraPosition;

// Simple point light structure
struct PointLight
{
	vec3 Position;
	vec3 Color;
};

// We'll support a max of 4 point lights for now
uniform PointLight u_PointLights[4];
uniform int u_NumPointLights;

// The final color of the fragment
out vec4 o_Color;

// Simplified PBR lighting model
vec3 CalculatePBR(vec3 N, vec3 V, vec3 albedo, float metallic, float roughness, float ao)
{
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);
    
    vec3 Lo = vec3(0.0);
    
    for(int i = 0; i < u_NumPointLights; ++i)
    {
        // Light vector and distance
        vec3 L = u_PointLights[i].Position - v_WorldPosition;
        float distance = length(L);
        L = normalize(L);
        
        // Attenuation
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = u_PointLights[i].Color * attenuation;
        
        // Simplified PBR (for now)
        vec3 halfwayDir = normalize(L + V);
        
        vec3 fresnel = F0 + (1.0 - F0) * pow(1.0 - max(dot(V, halfwayDir), 0.0), 5.0);
        
        vec3 kD = mix(vec3(1.0 - fresnel), vec3(0.0), metallic);
        vec3 diffuse = kD * albedo / PI;
        
        float NDF = 1.0 / (roughness * roughness);
        float G = 1.0 / 2.0;
        
        vec3 specular = fresnel;
        
        Lo += (kD * diffuse + specular) * radiance * max(0.0, dot(N, L));
    }
    
    // Ambient lighting
    vec3 ambient = 0.03 * albedo * ao;
    
    return ambient + Lo;
}

void main()
{
	// Normalize the normal vector
	vec3 N = normalize(v_Normal);
	
	// Calculate the view direction from the fragment to the camera
	vec3 V = normalize(u_CameraPosition - v_WorldPosition);
	
	vec3 result = CalculatePBR(N, V, u_Albedo, u_Metallic, u_Roughness, u_AO);
	
	o_Color = vec4(result, 1.0);
}
