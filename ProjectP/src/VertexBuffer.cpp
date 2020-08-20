#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* a_data, unsigned int a_size)
{
	/* Generates a Buffer ID using the glGenBuffers (VBO - Vertex Buffer Object) */
	glGenBuffers(1, &m_RendererID);
	/* GL_ARRAY_BUFFER is a Vertex Buffer Object */
	/* From this point, any buffer calls we make (on GL_ARRAY_BUFFER) will be used to configure - */
	/* The current bound Buffer Object (Which is VBO) */
    /* glBindBuffer binds the newly created Buffer to the Buffer Object (GL_ARRAY_BUFFER) */
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	/* glBufferData function copies the Vertex Data into the Buffers memory (Puts the memory from the CPU into the GPU) */
	glBufferData(GL_ARRAY_BUFFER, a_size, a_data, GL_DYNAMIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::UnBind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}