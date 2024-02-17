#version 450 core

in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

out vec4 fragColor;

uniform vec4 diffuse;
uniform sampler2D albedoMap;

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

vec3 CalcDirectionalLight(DirectionalLight light, vec3 N, vec3 albedo)
{
    vec3 L = normalize(light.direction);

    float lambert = max(dot(N, L), 0.1f);
	float intensity = light.intensity * lambert;
	return light.color * intensity;
}

vec3 CalcPointLight(PointLight light, vec3 N, vec3 albedo)
{
    vec3 L = normalize(light.position - fragPos);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)) + 1;
    vec3 radiance = light.color * light.intensity * attenuation;

    float lambert = max(dot(N, L), 0.1f);
    return radiance * lambert;
}

void main() 
{
    int numPointLights = min(activePointLights, MAX_POINT_LIGHTS);

    vec4 albedo = texture(albedoMap, texCoord) * diffuse;
    vec3 normal = normalize(normal);
    vec3 result = CalcDirectionalLight(directionalLight, normal, albedo.rgb);

    for (int i = 0; i < numPointLights; ++i) 
    {
	    result += CalcPointLight(pointLights[i], normal, albedo.rgb);
    }

    result = albedo.rgb * result;
    fragColor = vec4(result.rgb, 1.0f);
}
