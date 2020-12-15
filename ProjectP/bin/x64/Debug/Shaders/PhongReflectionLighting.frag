#version 460

layout(location = 0) out vec4 FragColor;

in vec3 m_lightIntensity;

void main()
{
   FragColor = vec4(m_lightIntensity, 1.0f);
}