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
uniform float roughnessStrength;
uniform float metallicStrength;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D roughnessMap;
uniform sampler2D metallicMap;
uniform sampler2D aoMap;

uniform vec3 cameraPos;

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
    Lo += CalcDirectionalLight(directionalLight, N, V, albedo, roughness, metalness, F0)

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

    vec3 color = indirectLight + Lo;
    color = ToneMapACES(color);
    color = pow(vec3(color), vec3(1.0/2.2)); // gamma correction

    fragColor = vec4(vec3(color), 1.0);
}
