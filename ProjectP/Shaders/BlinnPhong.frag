#version 460

in vec3 m_normal;
in vec3 m_position;

layout(location = 0) out vec4 FragColor;

uniform struct LightInfo
{
	vec4 u_position; /*Light Position in Eye Coords*/
	vec3 u_la;		 /*Ambient Light Intensity*/
	vec3 u_l;		 
}Light;

uniform struct MaterialInfo
{
	vec3 u_ka; /*Ambient Reflectivity*/
	vec3 u_kd; /*Diffuse Reflectivity*/
	vec3 u_ks; /*Specular Reflectivity*/
	float u_shininess; /*Specular Shininess Factor*/
}Material;

vec3 BlingPhong(vec3 position, vec3 n)
{
	vec3 m_ambient = Light.u_la * Material.u_ka;
	vec3 m_s = normalize(Light.u_position.xyz - position);
	float m_sDotN = max(dot(m_s, n), 0.0f);
	vec3 m_diffuse = Material.u_kd * m_sDotN;
	vec3 m_spec = vec3(0.0f);
	if(m_sDotN > 0.0f)
	{
		vec3 m_v = normalize(-position.xyz);
		vec3 m_h = normalize(m_v + m_s);
		m_spec = Material.u_ks * pow(max(dot(m_h, n), 0.0f), Material.u_shininess);
	}
	return m_ambient + Light.u_l * (m_diffuse + m_spec);
}

void main()
{
	FragColor = vec4(BlingPhong(m_position, normalize(m_normal)), 1.0f);
}
