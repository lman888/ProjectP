#version 460 core

layout(location = 0) out vec4 FragColor;

in vec3 LightIntensity;

void main()
{
   FragColor = vec4(LightIntensity, 1.0f);
}