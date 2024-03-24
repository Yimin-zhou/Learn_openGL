#version 450 core

in VS_OUT
{
	vec3 fragPosLocal;
	vec3 fragPosWorld;
    vec4 fragPosLightSpace;
	vec3 fragNormalWorld;
	vec2 fragTexCoord;
    vec3 fragTangentWorld;
    mat3 TBN_World;
} fs_in;

out vec4 fragColor;

uniform vec4 baseColor;
uniform float roughnessStrength;
uniform float metallicStrength;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D roughnessMap;
uniform sampler2D metallicMap;
uniform sampler2D aoMap;

uniform vec3 cameraPos;

uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;

uniform sampler2D shadowMap;


// Directional light struct
struct DirectionalLight 
{
    vec3 direction;
    vec3 color;
    float intensity;
};

// Point light struct
struct PointLight 
{
    vec3 position;
    vec3 color;
    float intensity;
    float constant;
    float linear;
    float quadratic;
    float radius;
};

// Spot light struct
struct SpotLight 
{
    vec3 position;
    vec3 direction;
    vec3 color;
    float intensity;
    float constant;
    float linear;
    float quadratic;
    float innerCutoff;
    float outerCutoff;
};

// define MAX POINT LIGHTS
#define MAX_POINT_LIGHTS 4
#define MAX_SPOT_LIGHTS 4

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int activePointLights;
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float a)
{
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float nom    = a2;
    float denom  = (NdotH2 * (a2 - 1.0) + 1.0);
    denom        = PI * denom * denom + 0.0001;
	
    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float k)
{
    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k  + 0.0001;
	
    return nom / denom;
}
  
float GeometrySmith(vec3 N, vec3 V, vec3 L, float k)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = GeometrySchlickGGX(NdotV, k);
    float ggx2 = GeometrySchlickGGX(NdotL, k);
	
    return ggx1 * ggx2;
}

// From UE4
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(2, ((-5.55473 * cosTheta) - 6.68316) * cosTheta);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(2, ((-5.55473 * cosTheta) - 6.68316) * cosTheta);;
}  

vec3 CalcDirectionalLight(DirectionalLight light, vec3 N, vec3 V, vec3 albedo, float roughness, float metalness, vec3 F0)
{
    vec3 L = normalize(light.direction);
    vec3 H = normalize(V + L);
    vec3 radiance = light.color * light.intensity;
        
    // cook-torrance brdf
    float D = DistributionGGX(N, H, roughness);        
    float G   = GeometrySmith(N, V, L, roughness);      
    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);       
        
    // Metal has no diffuse component
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metalness;	  
        
    vec3 numerator    = D * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular     = numerator / denominator;  

    float NdotL = max(dot(N, L), 0.0);   
    return (kD * albedo / PI + specular) * radiance * NdotL;
}

vec3 CalcPointLight(PointLight light, vec3 N, vec3 V, vec3 albedo, float roughness, float metalness, vec3 F0)
{
    vec3 L = normalize(light.position - fs_in.fragPosWorld);
    vec3 H = normalize(V + L);
    float distance = length(light.position - fs_in.fragPosWorld);
    float lightRadius = light.radius;
    // Inverse square falloff achieves more natural results
    float falloff = pow(clamp(1.0 - pow(distance / lightRadius, 4.0), 0.0, 1.0), 2.0);
    float attenuation = falloff / (distance * distance + 1.0);
    vec3 radiance = light.color * light.intensity * attenuation;
//    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
//    vec3 radiance = light.color * light.intensity * attenuation;
        
    // cook-torrance brdf
    float NDF = DistributionGGX(N, H, roughness);        
    float G   = GeometrySmith(N, V, L, roughness);      
    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);       
        
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metalness;	  
        
    vec3 numerator    = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular     = numerator / denominator;  

    float NdotL = max(dot(N, L), 0.0);   
    return (kD * albedo / PI + specular) * radiance * NdotL;
}

vec3 CalcSpotLight(SpotLight light, vec3 N, vec3 V, vec3 albedo, float roughness, float metalness, vec3 F0)
{
    vec3 L = normalize(light.position - fs_in.fragPosWorld);
    vec3 H = normalize(V + L);
    float distance = length(light.position - fs_in.fragPosWorld);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    float theta = dot(light.direction, normalize(-L)); // Angle between light direction and direction to fragment
    float falloff = 1.0;
    if (theta > light.outerCutoff) 
    {
        falloff = (theta - light.outerCutoff) / (light.innerCutoff - light.outerCutoff); // Smooth falloff between inner and outer cones
    } 
    else 
    {
        falloff = 0.0; // Outside the outer cone, no light
    }

    vec3 radiance = light.color * light.intensity * falloff * attenuation;
        
    // cook-torrance brdf
    float NDF = DistributionGGX(N, H, roughness);        
    float G   = GeometrySmith(N, V, L, roughness);      
    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);       
        
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metalness;	  
        
    vec3 numerator    = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular     = numerator / denominator;  

    float NdotL = max(dot(N, L), 0.0);   
    return (kD * albedo / PI + specular) * radiance * NdotL;
}

vec3 ToneMapACES(vec3 color)
{
    const float A = 2.51;
    const float B = 0.03;
    const float C = 2.43;
    const float D = 0.59;
    const float E = 0.14;
    return clamp((color * (A * color + B)) / (color * (C * color + D) + E), 0.0, 1.0);
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float shadow = currentDepth > closestDepth ? 1.0 : 0.0;

    return shadow;
}

void main()
{
    vec3 albedo     = pow(texture(albedoMap, fs_in.fragTexCoord).rgb, vec3(2.2)) * baseColor.rgb; // srgb texture, remove gamma correction, cause light is cauculated in linear
    float roughness = clamp(texture(roughnessMap, fs_in.fragTexCoord).r * roughnessStrength,0.0, 1.0);
    float metalness  = clamp(texture(metallicMap, fs_in.fragTexCoord).r * metallicStrength,0.0, 1.0);
    float ao        = texture(aoMap, fs_in.fragTexCoord).g;
    vec3 normal = normalize(fs_in.TBN_World * (texture(normalMap, fs_in.fragTexCoord).rgb * 2.0 - 1.0));

    vec3 N = normal;
    vec3 V = normalize(cameraPos - fs_in.fragPosWorld);

    vec3 F0 = vec3(0.04); // common value for non-metallic objects
    F0 = mix(F0, albedo, metalness);

    vec3 Lo = vec3(0.0);

    // direct

    // directional light
    Lo += CalcDirectionalLight(directionalLight, N, V, albedo, roughness, metalness, F0);

    // Shadow calculation.
    float shadow = ShadowCalculation(fs_in.fragPosLightSpace);

    // point
    int numPointLights = min(activePointLights, MAX_POINT_LIGHTS);
    for(int i = 0; i < numPointLights; i++)
	{
		Lo += CalcPointLight(pointLights[i], N, V, albedo, roughness, metalness, F0);
	}

    // spot
//    for(int i = 0; i < MAX_SPOT_LIGHTS; i++)
//    {
//        Lo += CalcSpotLight(spotLights[i], N, V, albedo, roughness, metalness, F0);
//    }
//

    // indirect (single scattering)
//    vec3 irradiance = texture(irradianceMap, normal).rgb;
//    vec3 ks = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
//    vec3 kd = 1.0 - ks;
//    vec3 diffuse = kd * albedo * irradiance * (1 - metalness);
//
//    // IBL (image based lighting)
//    vec3 R = reflect(-V, N);
//    float mipLevel = roughness * 4.0;
//    vec3 prefilteredColor = textureLod(prefilterMap, R, mipLevel).rgb;
//    vec2 envBRDF = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
//    vec3 specular = prefilteredColor * (ks * envBRDF.x + envBRDF.y);
//
//    vec3 indirectLight = (diffuse + specular) * ao;

    // indirect (multiple scattering)
    vec3 irradiance = texture(irradianceMap, normal).rgb;
    vec3 ks = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
    vec3 R = reflect(-V, N);
    float mipLevel = roughness * 4.0;
    vec3 prefilteredColor = textureLod(prefilterMap, R, mipLevel).rgb;
    vec2 envBRDF = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;

    vec3 FssEss = ks * envBRDF.x + envBRDF.y;

    float Ems = (1.0 - (envBRDF.x + envBRDF.y));
    vec3 Favg = F0 + (1.0 - F0) / 21.0;
    vec3 FmsEms = Ems * FssEss * Favg / (1.0 - Favg * Ems);
    vec3 kd = albedo * (1.0 - FssEss - FmsEms) * (1.0 - metalness);
    vec3 indirectLight = (FssEss * prefilteredColor + (FmsEms + kd) * irradiance) * ao;

    vec3 color = (indirectLight + Lo) * (1.0 - shadow);
    color = ToneMapACES(color);
    color = pow(vec3(color), vec3(1.0/2.2)); // gamma correction

    fragColor = vec4(vec3(color), 1.0);
}
