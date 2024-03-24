#version 450 core
layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

uniform mat4 modelMatrix;
uniform mat4 projectionViewMatrix;
uniform mat4 lightSpaceMatrix;

out VS_OUT
{
	vec3 fragPosLocal;
	vec3 fragPosWorld;
    vec4 fragPosLightSpace;
	vec3 fragNormalWorld;
	vec2 fragTexCoord;
    vec3 fragTangentWorld;
    mat3 TBN_World;
} vs_out;

void main() 
{
    gl_Position = projectionViewMatrix * modelMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    vs_out.fragPosLocal = aPos;
    vs_out.fragPosWorld = (modelMatrix * vec4(aPos, 1.0)).xyz;
    vs_out.fragPosLightSpace = (lightSpaceMatrix * modelMatrix * vec4(aPos, 1.0));
    vs_out.fragTexCoord = vec2(aTexCoord.x, aTexCoord.y);
    vs_out.fragNormalWorld = normalize((modelMatrix * vec4(aNormal, 0.0)).xyz);
    vs_out.fragTangentWorld = normalize((modelMatrix * vec4(aTangent, 0.0)).xyz);

	vec3 bitangent = normalize((modelMatrix * vec4(aBitangent, 0.0)).xyz);
	vs_out.TBN_World = mat3(vs_out.fragTangentWorld, bitangent, vs_out.fragNormalWorld);

}