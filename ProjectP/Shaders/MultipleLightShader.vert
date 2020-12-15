#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 m_Color;

uniform struct LightInfo
{
	vec4 u_position; /*Light Position in Eye Coords*/
	vec3 u_la;		 /*Ambient Light Intensity*/
	vec3 u_l;		 /*Diffuse and Specular Light Intensity*/
}Lights[5];

uniform struct MaterialInfo
{
	vec3 u_ka; /*Ambient Reflectivity*/
	vec3 u_kd; /*Diffuse Reflectivity*/
	vec3 u_ks; /*Specular Reflectivity*/
	float u_shininess; /*Specular Shininess Factor*/
}Material;

uniform mat4 u_modelViewMatrix;
uniform mat3 u_normalMatrix;
uniform mat4 u_projectionMatrix;
uniform mat4 u_MVP;

void GetCamSpace(out vec3 norm, out vec3 position)
{
	norm = normalize(u_normalMatrix * VertexNormal);
	position = (u_modelViewMatrix * vec4(VertexPosition, 1.0f)).xyz;
}

vec3 PhongModel(int Light, vec3 position, vec3 n)
{
	vec3 ambient = Lights[Light].u_la * Material.u_ka;
	vec3 s = normalize(vec3(Lights[Light].u_position.xyz - position));
	float sDotN = max(dot(s, n), 0.0f);
	vec3 diffuse = Material.u_kd * sDotN;
	vec3 spec = vec3(0.0f);
	if(sDotN > 0.0f)
	{
		vec3 v = normalize(-position.xyz);
		vec3 r = reflect(-s, n);
		spec = Material.u_ks *
			   pow(max(dot(r, v), 0.0f), Material.u_shininess);
	}
	return ambient + Lights[Light].u_l * (diffuse + spec);
}

void main()
{
	/*Get the position and normal in cam space*/
	vec3 camNorm, camPosition;
	GetCamSpace(camNorm, camPosition);
	
	/*Evaluate the Lighting Equation for each Light*/
	m_Color = vec3(0.0);
	for(int i = 0; i < 5; i++)
	{
		m_Color += PhongModel(i, camPosition, camNorm);
	}
	
	gl_Position = u_MVP * vec4(VertexPosition, 1.0f);
}