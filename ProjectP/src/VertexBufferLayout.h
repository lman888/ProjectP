#pragma once

#include <vector>
#include "GLAD/glad.h"

struct VertexBufferLayoutElement
{
	unsigned int  m_Type;
	unsigned int  m_Count;
	unsigned char m_Noramlized;

	static unsigned int GetSizeOfType(unsigned int a_type)
	{
		switch (a_type)
		{
			case GL_FLOAT:		   return 4;
			case GL_UNSIGNED_INT:  return 4;
			case GL_INT:		   return 4;
			case GL_UNSIGNED_BYTE: return 1;
		}
		_ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
public: 

	VertexBufferLayout()
		 : m_Stride(0){}
	~VertexBufferLayout() {}

	template<typename T>
	void Push(unsigned int a_count)
	{
		static_assert(false);
	}

	/* Maintains the ELements that make up our Vertex Buffer */
	template<>
	void Push<float>(unsigned int a_count)
	{
		m_Elements.push_back({ GL_FLOAT, a_count, GL_FALSE });
		m_Stride += a_count * VertexBufferLayoutElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(unsigned int a_count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, a_count, GL_FALSE });
		m_Stride += a_count * VertexBufferLayoutElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(unsigned int a_count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, a_count, GL_TRUE });
		m_Stride += a_count * VertexBufferLayoutElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<VertexBufferLayoutElement> GetElements() const& { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride; }

protected:



private:

	/* Vector that stores all of our Buffer Layout Elements */
	std::vector<VertexBufferLayoutElement> m_Elements;
	unsigned int m_Stride;

};

