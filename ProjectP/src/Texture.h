#pragma once

#include "Renderer.h"

class Texture
{
public:

	Texture(const std::string a_path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void UnBind();

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }

	unsigned int m_RendererID;

protected:

private:

	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	/* Width, Height, Bits-Per-Pixel */
	int m_Width, m_Height, m_BPP;

};