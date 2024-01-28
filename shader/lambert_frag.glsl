#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform vec4 diffuse;
uniform sampler2D albedoMap;

void main() 
{
    vec4 albedo = texture(albedoMap, TexCoord) * diffuse;
    FragColor = vec4(albedo.rgb, 1.0f);
}
