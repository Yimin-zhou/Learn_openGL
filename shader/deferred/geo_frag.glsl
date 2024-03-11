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

layout (location = 0) out vec4 gWorlPos;
layout (location = 1) out vec4 gWorldNormal;
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out vec4 gRoughnessMetalnessAo;
layout (location = 4) out vec4 gEmission; 

uniform vec4 baseColor;
uniform float roughnessStrength;
uniform float metallicStrength;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D roughnessMap;
uniform sampler2D metallicMap;
uniform sampler2D aoMap;

void main()
{
    vec3 albedo     = pow(texture(albedoMap, fs_in.fragTexCoord).rgb, vec3(2.2)) * baseColor.rgb; // srgb texture, remove gamma correction, cause light is cauculated in linear
    float roughness = clamp(texture(roughnessMap, fs_in.fragTexCoord).r * roughnessStrength,0.0, 1.0);
    float metalness  = clamp(texture(metallicMap, fs_in.fragTexCoord).r * metallicStrength,0.0, 1.0);
    float ao        = texture(aoMap, fs_in.fragTexCoord).g;
    vec3 normal = normalize(fs_in.TBN_World * (texture(normalMap, fs_in.fragTexCoord).rgb * 2.0 - 1.0));

    gWorlPos = vec4(fs_in.fragPosWorld, 1.0);
    gWorldNormal = vec4(normal, 1.0);
    gAlbedo = vec4(albedo, 1.0);
    gRoughnessMetalnessAo = vec4(roughness, metalness, ao, 1.0);
    gEmission = vec4(0.0, 0.0, 0.0, 1.0);
}
