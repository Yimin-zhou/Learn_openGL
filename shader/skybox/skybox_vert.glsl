#version 450 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

uniform float time;
uniform float speed;

void main()
{
    TexCoords = aPos;  
    // spin the skybox along the time
	mat4 modelMatrix = mat4(
		vec4(cos(time * speed), 0.0, sin(time * speed), 0.0),
		vec4(0.0, 1.0, 0.0, 0.0),
		vec4(-sin(time * speed), 0.0, cos(time * speed), 0.0),
		vec4(0.0, 0.0, 0.0, 1.0)
	);

    gl_Position = projection * view * modelMatrix * vec4(aPos, 1.0);
    gl_Position = gl_Position.xyww; // Remove perspective divide to keep the skybox at the same distance from the camera
}
