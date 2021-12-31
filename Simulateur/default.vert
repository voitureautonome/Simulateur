#version 330 core


layout (location = 0) in vec3 aPos;

layout (location = 1) in vec3 aColor;
// Texture Coordinates
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec3 aNormal;

// Outputs the color for the Fragment Shader
out vec3 color;
// Outputs the texture coordinates to the fragment shader
out vec2 texCoord;

out vec3 Normal;
// Outputs the current position for the Fragment Shader
out vec3 crntPos;

uniform mat4 model;
uniform mat4 camMatrix;

void main()
{
	crntPos = vec3(model * vec4(aPos, 1.0f));

	gl_Position = camMatrix * model * vec4(aPos, 1.0);

	color = aColor;

	texCoord = aTex;
	Normal = aNormal;
}