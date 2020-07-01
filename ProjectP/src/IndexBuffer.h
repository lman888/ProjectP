#pragma once

class IndexBuffer
{
public:

	IndexBuffer(const unsigned int* a_data, unsigned int a_count);
	~IndexBuffer();

	void Bind() const;
	void UnBind() const;

	inline unsigned int GetCount() const { return m_Count; }

protected:

private:
	
	unsigned int m_RendererID;
	unsigned int m_Count;
};