#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoords;

out vec3 m_frontColor;
out vec3 m_backColor;
out vec2 m_texCoord;

uniform struct LightInfo
{
	vec4 u_position; /*Light Position in Eye Coords*/
	vec3 u_la;		 /*Ambient Light Intensity*/
	vec3 u_ld;		 /*Diffuse Light Intensity*/
	vec3 u_ls;		 /*Specular Light Intensity*/
}Light;

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

vec3 PhongModel(vec3 position, vec3 n)
{
	vec3 ambient = Light.u_la * Material.u_ka;
	vec3 s = normalize(vec3(Light.u_position.xyz - position));
	float sDotN = max(dot(s, n), 0.0f);
	vec3 diffuse = Light.u_ld * Material.u_kd * sDotN;
	vec3 spec = vec3(0.0f);
	if(sDotN > 0.0f)
	{
		vec3 v = normalize(-position.xyz);
		vec3 r = reflect(-s, n);
		spec = Light.u_ls * Material.u_ks *
			   pow(max(dot(r, v), 0.0f), Material.u_shininess);
	}
	return ambient + diffuse + spec;
}

void GetCamPosition(out vec3 a_norm, out vec3 position)
{
	/*Get the position and normal in cam space*/
	a_norm = normalize(u_normalMatrix * VertexNormal);
	position = (u_modelViewMatrix * vec4(VertexPosition, 1.0f)).xyz;
}

void main()
{	

	m_texCoord = VertexTexCoords;
	vec3 m_tnorm, m_camCoords;
	
	GetCamPosition(m_tnorm, m_camCoords);
	
	/*Evaluate the reflection Model*/
	m_frontColor = PhongModel(m_camCoords, m_tnorm);
	m_backColor = PhongModel(m_camCoords, -m_tnorm);
	
	gl_Position = u_MVP * vec4(VertexPosition, 1.0f);
}