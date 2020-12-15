#version 460

layout(location = 0) out vec4 FragColor;

in vec3 m_Color;

void main()
{
   FragColor = vec4(m_Color, 1.0f);
}