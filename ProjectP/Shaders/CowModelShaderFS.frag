#version 460 core
out vec4 FragColour;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{
	FragColour = vec4(texture(texture_diffuse1, TexCoords));
}