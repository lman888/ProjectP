#version 460

in vec3 m_normal;
in vec3 m_position;

layout(location = 0) out vec4 FragColor;

uniform struct SpotLight
{
	vec3 u_position;  /*Light Position in Eye Coords*/
	vec3 u_la;		  /*Ambient Light Intensity*/
	vec3 u_l; 		  /*Diffuse/Specular Intensity*/ 
	vec3 u_direction; /*Direction of the SpotLight in CamCoords*/
	float u_exponent; /*Angular Attenuation Exponent*/
	float u_cutOff;   /*CutOff Angle (Between 0 and pi/2)*/
}Spot;

uniform struct MaterialInfo
{
	vec3 u_ka; /*Ambient Reflectivity*/
	vec3 u_kd; /*Diffuse Reflectivity*/
	vec3 u_ks; /*Specular Reflectivity*/
	float u_shininess; /*Specular Shininess Factor*/
}Material;

vec3 BlinnPhongSpotLight(vec3 position, vec3 n)
{
	vec3 ambient = Spot.u_la * Material.u_ka, 
	diffuse = vec3(0), spec = vec3(0);
	vec3 s = normalize(Spot.u_position - position);
	float cosAng = dot(-s, normalize(Spot.u_direction));
	float angle = acos(cosAng);
	float spotScale = 0.0f;
	
	if(angle < Spot.u_cutOff)
	{
		spotScale = pow(cosAng, Spot.u_exponent);
		float sDotN = max(dot(s, n), 0.0f);
		diffuse = Material.u_kd * sDotN;
		
		if(sDotN > 0.0f)
		{
			vec3 v = normalize(-position.xyz);
			vec3 h = normalize(v + s);
			spec = Material.u_ks * pow(max(dot(h, n), 0.0f), Material.u_shininess);
		}
	}
	return ambient + spotScale * Spot.u_l * (diffuse + spec);
}

void main()
{
   FragColor = vec4(BlinnPhongSpotLight(m_position, normalize(m_normal)), 1.0f);
}