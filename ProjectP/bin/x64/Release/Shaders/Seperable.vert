#version 460

layout (location = 0) vec3 VertexPosition;
layout (location = 1) vec3 VertexColor;

uniform vec3 u_ColorMask = vec3(0,0);

layout (location = 0) out vec3 Color;

out gl_PerVertex
{
	vec4 gl_Position;
	gloat gl_PointSize;
	float gl_ClipDistance[];
};

void main()
{
	VertexColor = VertexColor * u_ColorMask;
	
    gl_Position = vec4(VertexPosition, 1.0);
}