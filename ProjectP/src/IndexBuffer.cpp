#include "IndexBuffer.h"
#include "Renderer.h"


IndexBuffer::IndexBuffer(unsigned int* a_data, unsigned int a_count)
	: m_Count(a_count)
{
	/* Generates a Buffer ID using the glGenBuffers (VBO - Vertex Buffer Object) */
	GLCall(glGenBuffers(1, &m_RendererID));
	/* GL_ARRAY_BUFFER is a Vertex Buffer Object */
	/* From this point, any buffer calls we make (on GL_ARRAY_BUFFER) will be used to configure - */
	/* The current bound Buffer Object (Which is VBO) */
	/* glBindBuffer binds the newly created Buffer to the Buffer Object (GL_ARRAY_BUFFER) */
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	/* glBufferData function copies the Vertex Data into the Buffers memory */
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, a_count * sizeof(unsigned int), a_data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void IndexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::UnBind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}