#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 m_position;
out vec3 m_normal;

uniform mat4 u_modelViewMatrix;
uniform mat3 u_normalMatrix;
uniform mat4 u_projectionMatrix;
uniform mat4 u_MVP;

void main()
{
	m_normal = normalize(u_normalMatrix * VertexNormal);
	m_position = (u_modelViewMatrix * vec4(VertexPosition, 1.0f)).xyz;
	
	gl_Position = u_MVP * vec4(VertexPosition, 1.0f);
}