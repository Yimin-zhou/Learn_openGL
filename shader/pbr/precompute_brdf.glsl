#version 450 core

layout (local_size_x = 16, local_size_y = 16) in;

layout (rgba16f, binding = 0) writeonly uniform image2D brdfLUT;

const float PI = 3.14159265359;

vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
{
    float a = roughness*roughness;
	
    float phi = 2.0 * PI * Xi.x;
    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
    float sinTheta = sqrt(1.0 - cosTheta*cosTheta);
	
    // from spherical coordinates to cartesian coordinates
    vec3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sin(phi) * sinTheta;
    H.z = cosTheta;
	
    // from tangent-space vector to world-space sample vector
    vec3 up        = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0); // this is to avoid the up vector and N vector are parallel
    vec3 tangent   = normalize(cross(up, N));
    vec3 bitangent = cross(N, tangent);
	
    vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
    return normalize(sampleVec);
}  

vec2 Hammersley(uint i, uint N)
{
    uint bits = (i << 16u) | (i >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    float rdi = float(bits) * 2.3283064365386963e-10;
    return vec2(float(i) /float(N), rdi);
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float a = roughness;
    float k = (a * a) / 2.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}  

// Karis 2014
vec2 IntegrateBRDF(float roughness, float NdotV)
{
	vec3 V;
    V.x = sqrt(1.0 - NdotV * NdotV); // sin
    V.y = 0.0;
    V.z = NdotV; // cos

    // N points straight upwards for this integration
    const vec3 N = vec3(0.0, 0.0, 1.0);

    float A = 0.0;
    float B = 0.0;
    const uint numSamples = 1024u;

    for (uint i = 0u; i < numSamples; i++) {
        vec2 Xi = Hammersley(i, numSamples);
        vec3 H = ImportanceSampleGGX(Xi, N, roughness);
        vec3 L = 2.0 * dot(V, H) * H - V;

        float NdotL = max(dot(N, L), 0);
        float NdotH = max(dot(N, H), 0);
        float VdotH = max(dot(V, H), 0);

        if (NdotL > 0.0) 
        {
            float G = GeometrySmith(N, V, L, roughness);
            float G_Vis = (G * VdotH) / (NdotH * NdotV);
            float Fc = pow(1.0 - VdotH, 5.0);
            A += (1.0 - Fc) * G_Vis;
            B += Fc * G_Vis;
        }
    }

//    return 4.0 * vec2(A, B) / float(numSamples);
	return vec2(A, B) / float(numSamples);
}

void main() 
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = vec2(gl_GlobalInvocationID.xy + 1) / vec2(imageSize(brdfLUT).xy);

    vec2 integratedBRDF = IntegrateBRDF(uv.x, uv.y);
    imageStore(brdfLUT, ivec2(gl_GlobalInvocationID.xy), vec4(integratedBRDF, 0.0, 0.0));
}
