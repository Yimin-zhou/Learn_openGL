#version 450 core

layout (local_size_x = 16, local_size_y = 16) in;

layout(binding = 0) uniform samplerCube environmentMap;
layout (rgba8, binding = 1) writeonly uniform imageCube prefilteredMap;

uniform float roughness;

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

void main() 
{
    ivec3 globalId = ivec3(gl_GlobalInvocationID.xyz);
    int face = globalId.z;
    vec2 uv = (vec2(globalId.xy) + vec2(0.5)) / vec2(imageSize(prefilteredMap).xy);
    uv = uv * 2.0 - 1.0; // Transform uv to [-1, 1]

    vec3 N;
    if (face == 0) N = vec3(1, -uv.y, -uv.x); // Positive X
    else if (face == 1) N = vec3(-1, -uv.y, uv.x); // Negative X
    else if (face == 2) N = vec3(uv.x, 1, uv.y); // Positive Y
    else if (face == 3) N = vec3(uv.x, -1, -uv.y); // Negative Y
    else if (face == 4) N = vec3(uv.x, -uv.y, 1); // Positive Z
    else if (face == 5) N = vec3(-uv.x, -uv.y, -1); // Negative Z

    vec3 R = N;
    vec3 V = R;
    float totalWeight = 0.0;
    vec3 prefilteredColor = vec3(0.0);
    uint numSamples = 1024; // Number of samples for the prefiltering
    for (uint i = 0; i < numSamples; ++i) 
    {
        vec2 Xi = Hammersley(i, numSamples);
        vec3 H = ImportanceSampleGGX(Xi, N, roughness);
        vec3 L = 2.0 * dot(V, H) * H - V; // Compute the reflection vector, L is the vector from the point to the light source
        float NdotL = max(dot(N, L), 0.0);
        if (NdotL > 0.0) 
        {
            prefilteredColor += texture(environmentMap, L).rgb * NdotL;
            totalWeight += NdotL;
        }
    }
    prefilteredColor /= totalWeight;
    imageStore(prefilteredMap, ivec3(gl_GlobalInvocationID.xyz), vec4(prefilteredColor, 1.0));
}
