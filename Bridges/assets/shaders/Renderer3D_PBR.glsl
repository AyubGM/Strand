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
layout(std140, binding = 1) uniform ObjectData
{
    mat4 u_Model;
    mat4 u_NormalMatrix; 

};

// Output to the fragment shader
layout(location = 0) out vec3 v_WorldPosition;
layout(location = 1) out vec3 v_Normal;
layout(location = 2) out vec2 v_TexCoord;

void main()
{
	// Calculate the world-space position of the vertex
	v_WorldPosition = vec3(u_Model * vec4(a_Position, 1.0));
	
	// Calculate the normal vector in world space
	v_Normal = mat3(u_NormalMatrix) * a_Normal;

	// Pass through the texture coordinates
	v_TexCoord = a_TexCoord;
	
	// Final position of the vertex on the screen
	gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 o_Color;
const float PI = 3.14159265359;

// Input from the vertex shader
layout(location = 0) in vec3 v_WorldPosition;
layout(location = 1) in vec3 v_Normal;
layout(location = 2) in vec2 v_TexCoord;

// Per-object material properties
layout(std140, binding = 2) uniform MaterialData
{
    vec4 u_Albedo;
    float u_Metallic;
    float u_Roughness;
    float u_AO;
};

// Simple point light structure
struct PointLight
{
	vec3 Position;
	vec3 Color;
};

// Scene-wide uniforms
layout(std140, binding = 3) uniform SceneData
{
    vec4 u_CameraPosition;
    // We'll support a max of 4 point lights for now
    int u_NumPointLights;
    PointLight u_PointLights[4];
};

// PBR Helper Functions based on standard microfacet theory

// Trowbridge-Reitz GGX for Normal Distribution Function (NDF)
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}


// Schlick-GGX for Geometry function (approximates self-shadowing)
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;
    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;
    return nom / denom;
}

// Smith's method for combining geometry functions
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
    return ggx1 * ggx2;
}

// Schlick's approximation for Fresnel
vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

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
vec3 N = normalize(v_Normal);
	vec3 V = normalize(u_CameraPosition - v_WorldPosition);

    // Base reflectivity: 4% for non-metals, albedo for metals
	vec3 F0 = vec3(0.04); 
	F0 = mix(F0, u_Albedo, u_Metallic);
	
	// Outgoing radiance (the final color we calculate)
	vec3 Lo = vec3(0.0);

	for(int i = 0; i < u_NumPointLights; ++i) 
	{
        // Light calculations
		vec3 L = normalize(u_PointLights[i].Position - v_WorldPosition);
		vec3 H = normalize(V + L);
		float distance = length(u_PointLights[i].Position - v_WorldPosition);
		float attenuation = 1.0 / (distance * distance);
		vec3 radiance = u_PointLights[i].Color * attenuation;
		
        // Cook-Torrance BRDF
		float NDF = DistributionGGX(N, H, u_Roughness);   
		float G   = GeometrySmith(N, V, L, u_Roughness);	  
		vec3  F   = FresnelSchlick(max(dot(H, V), 0.0), F0);		
		
        // Specular term
		vec3 numerator = NDF * G * F;
		float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // 0.0001 to prevent divide by zero
		vec3 specular = numerator / denominator;
			
        // Diffuse term (kS is used for energy conservation)
		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - u_Metallic; // Metals have no diffuse color
		
		float NdotL = max(dot(N, L), 0.0);		
		Lo += (kD * u_Albedo / PI + specular) * radiance * NdotL;
	}
	
    // Simple ambient light
	vec3 ambient = vec3(0.03) * u_Albedo * u_AO;
	vec3 color = ambient + Lo;
	
    // HDR tonemapping and gamma correction
	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0/2.2)); 
	
	o_Color = vec4(color, 1.0);
}
