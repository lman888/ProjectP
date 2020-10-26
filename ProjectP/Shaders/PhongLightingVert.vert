#version 460 core

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;

out vec3 LightIntensity;

uniform vec4 u_lightPosition; //Light Pos in Camera Coords
uniform vec3 u_kd;            //Diffuse Reflectivity
uniform vec3 u_ld;            //Light Source Intensity

uniform mat4 u_modelViewMatrix;
uniform mat3 u_normalMatrix;
uniform mat4 u_projMatrix;

uniform mat4 u_MVP;         //Matrix Projection View

void main()
{
	//Convert Normal and Position to eye Coords
	vec3 tnorm = normalize(u_normalMatrix * VertexNormal);
	vec4 camCoords = u_modelViewMatrix * vec4(VertexPosition, 1.0f);
	vec3 s = normalize(vec3(u_lightPosition - camCoords));

	//The diffuse Shader Equation
	LightIntensity = u_ld * u_kd * max(dot(s, tnorm), 0.0f);
	
	gl_Position = u_MVP * vec4(VertexPosition, 1.0f);
}