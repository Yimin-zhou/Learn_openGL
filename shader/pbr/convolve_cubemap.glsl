#version 450 core

layout(local_size_x = 16, local_size_y = 16) in;

layout(binding = 0) uniform samplerCube sourceCubemap;
layout(rgba8, binding = 1) writeonly uniform imageCube destinationCubemap;

#define PI 3.14159265359

void main() 
{
    ivec3 globalId = ivec3(gl_GlobalInvocationID.xyz);
    vec3 irradiance = vec3(0.0);  

    int face = globalId.z;
    vec2 uv = (vec2(globalId.xy) + vec2(0.5)) / vec2(imageSize(destinationCubemap).xy);
    uv = uv * 2.0 - 1.0; // Transform uv to [-1, 1]

    vec3 normal;
    if (face == 0) normal = vec3(1, -uv.y, -uv.x); // Positive X
    else if (face == 1) normal = vec3(-1, -uv.y, uv.x); // Negative X
    else if (face == 2) normal = vec3(uv.x, 1, uv.y); // Positive Y
    else if (face == 3) normal = vec3(uv.x, -1, -uv.y); // Negative Y
    else if (face == 4) normal = vec3(uv.x, -uv.y, 1); // Positive Z
    else if (face == 5) normal = vec3(-uv.x, -uv.y, -1); // Negative Z

    vec3 up = abs(normal.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent = normalize(cross(up, normal));
    vec3 bitangent = normalize(cross(normal, tangent));

    float sampleDelta = 0.025;
    float nrSamples = 0.0; 
    for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
    {
        for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
        {
            // spherical to cartesian (in tangent space)
            vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
            // tangent space to world
            vec3 sampleVec = tangentSample.x * tangent + tangentSample.y * bitangent + tangentSample.z * normal; 

            irradiance += texture(sourceCubemap, sampleVec).rgb * cos(theta) * sin(theta);
            nrSamples++;
        }
    }
    irradiance = PI * irradiance * (1.0 / float(nrSamples));
    imageStore(destinationCubemap, globalId, vec4(irradiance, 1));
}
