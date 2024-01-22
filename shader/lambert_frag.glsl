#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec4 diffuse;
uniform sampler2D albedoMap;

void main() 
{
    vec4 albedo = texture(albedoMap, TexCoords) * diffuse;
    FragColor = vec4(albedo.rgb, 1.0f);
}
