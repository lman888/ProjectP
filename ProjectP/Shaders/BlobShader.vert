#version 460 core

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexTexCoords;

// This Data is sent to the Fragment Shader for compilation
out vec3 TexCoord;
uniform mat4 u_MVP;

void main()
{
	TexCoord = VertexTexCoords;
	gl_Position = u_MVP * vec4(VertexPosition, 1.0f);
}