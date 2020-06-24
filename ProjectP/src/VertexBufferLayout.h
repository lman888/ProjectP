#pragma once

#include <vector>
#include "GLAD/glad.h"

struct VertexBufferLayoutElemet
{
	unsigned int m_Count;
	unsigned int m_Type;
	bool m_Noramlized;
};

class VertexBufferLayout
{
public: 

	VertexBufferLayout();
	~VertexBufferLayout();

	template<typename T>
	void Push(unsigned int a_count)
	{
		static_assert(false);
	}

	template<>
	void Push<float>(unsigned int a_count)
	{
		m_Element.push_back({GL_FLOAT, a_count, false});
	}

protected:

private:

	std::vector<VertexBufferLayoutElemet> m_Element;
};

