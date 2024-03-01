#version 450 core

in VS_OUT
{
	vec3 fragPosLocal;
	vec3 fragPosWorld;
	vec3 fragNormalWorld;
	vec2 fragTexCoord;
    vec3 fragTangentWorld;
    mat3 TBN_World;
} fs_in;

out vec4 fragColor;

uniform vec4 baseColor;
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D roughnessMap;
uniform sampler2D metallicMap;
uniform sampler2D aoMap;

uniform vec3 cameraPos;


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

// learnopengl: https://learnopengl.com/PBR/Theory
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

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
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
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    vec3 radiance = light.color * light.intensity * attenuation;
        
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

void main()
{
    vec3 albedo     = pow(texture(albedoMap, fs_in.fragTexCoord).rgb, vec3(2.2)) * baseColor.rgb; // srgb texture, remove gamma correction, cause light is cauculated in linear
    float roughness = texture(roughnessMap, fs_in.fragTexCoord).g;
    float metalness  = texture(metallicMap, fs_in.fragTexCoord).r;
    float ao        = texture(aoMap, fs_in.fragTexCoord).r;
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
//    vec4 shadowCoord = lightSpaceMatrix * vec4(fragPosition, 1.0); // Light space position
//    shadowCoord /= shadowCoord.w; // Perspective divide
//    shadowCoord = shadowCoord * 0.5 + 0.5; // Convert to [0, 1] range
//    float closestDepth = texture(shadowMap, shadowCoord.xy).r; // Read depth from texture
//    float currentDepth = shadowCoord.z; // Depth of current fragment in light space
//    float bias = 0.005; // Bias to avoid shadow acne
//    
//    float shadow = 0.0;
//    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
//    for(int x = -2; x <= 2; ++x)
//    {
//        for(int y = -2; y <= 2; ++y)
//        {
//            float pcfDepth = texture(shadowMap, shadowCoord.xy + vec2(x, y) * texelSize).r; 
//            shadow += currentDepth - bias> pcfDepth ? 0.0 : 1.0;        
//        }    
//    }
//    shadow /= 9.0;
//    Lo *= shadow;

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

    // indirect
    vec3 ambient = vec3(0.1) * albedo * ao; // TODO

    // IBL (image based lighting)
//    vec3 R = reflect(-V, N);
//    float mipLevel = roughness * float(mipCount - 1);
//    vec3 prefilteredColor = textureLod(prefilterMap, R, mipLevel).rgb;
//    vec2 envBRDF = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
//    vec3 irradiance = texture(irradianceMap, N).rgb;
//    vec3 diffuse = irradiance * albedo;
//    vec3 specular = prefilteredColor * (F0 * envBRDF.x + envBRDF.y);
//    Lo += (diffuse + specular) * ao;




    vec3 color = ambient + Lo;
	
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2)); // gamma correction

    fragColor = vec4(vec3(color), 1.0);
}
