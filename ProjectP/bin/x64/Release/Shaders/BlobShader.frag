#version 460

in vec3 TexCoord;
layout (location = 0) out vec4 FragColor;

layout (location = 1) uniform u_blobSettings
{
	vec4 innerColor;
	vec4 outerColor;
	float radiusInner;
	float radiusOuter;
} Blob;


void main()
{
	float dx = TexCoord.x - 0.5f;
	float dy = TexCoord.y - 0.5f;
	float dist = sqrt(dx * dx + dy * dy);
	
    FragColor = mix(Blob.innerColor, Blob.outerColor,
				smoothstep(Blob.radiusInner, Blob.radiusOuter, dist));
}