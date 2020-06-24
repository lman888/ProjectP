#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

/* Vertex Arrays are a way to bind Vertex Buffers with a certain specifications for the layout of the buffer */
class VertexArray
{
public:

	VertexArray();
	~VertexArray();

	/* Combines the data in Vertex Buffer and Buffer Layout to generate the Triangle */
	void AddBuffer(const VertexBuffer& a_VB, const VertexBufferLayout& a_Layout);

	void Bind() const;
	void UnBind() const;

protected:

private:

	unsigned int m_RendererID;

};

