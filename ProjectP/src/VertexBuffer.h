#pragma once

class VertexBuffer
{
public: 

	VertexBuffer(const void* a_data, unsigned int a_size);
	~VertexBuffer();

	void Bind() const;
	void UnBind() const;

protected: 

private:

	unsigned int m_RendererID;

};