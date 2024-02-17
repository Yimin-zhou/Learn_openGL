#version 450 core
layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 modelMatrix;
uniform mat4 viewProjectionMatrix;

out vec3 fragPos;
out vec3 normal;
out vec2 texCoord;

void main() 
{
    gl_Position = viewProjectionMatrix * modelMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    fragPos = aPos;
    normal = aNormal;
    texCoord = vec2(aTexCoord.x, aTexCoord.y);
}
