#version 460

layout(location = 0) out vec4 FragColor;

flat in vec3 m_frontColor;
flat in vec3 m_backColor;

void main()
{
   if(gl_FrontFacing)
   {
		FragColor = vec4(m_frontColor, 1.0f);
   }
   else
   {
		FragColor = vec4(m_backColor, 1.0f);
   }
}