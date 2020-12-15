#version 460

in vec3 m_frontColor;
in vec3 m_backColor;
in vec2 m_texCoord;

layout (location = 0) out vec4 FragColor;

void main()
{
    const float m_scale = 20.0f;
	
	bvec2 toDiscard = greaterThan( fract(m_texCoord * m_scale), vec2(0.2, 0.2) );
	
    if(all(toDiscard))
		discard;
	else
	{
		if(gl_FrontFacing)
		   FragColor = vec4(m_frontColor, 1.0f);
		else
		   FragColor = vec4(m_backColor, 1.0f);
	}
	
}