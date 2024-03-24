#version 450 core
layout (location = 0) in vec3 aPos;

uniform mat4 modelMatrix;
uniform mat4 projectionViewMatrix;

void main()
{
    gl_Position = projectionViewMatrix * modelMatrix * vec4(aPos, 1);
}
