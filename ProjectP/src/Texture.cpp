#include "Texture.h"

#include "Extra/stb_image.h"

Texture::Texture(const std::string a_path)
	: m_FilePath(a_path), m_LocalBuffer(nullptr), 
	  m_Width(0), m_Height(0), m_BPP(0)
{
	/* Flips the Texture Vertically */
	stbi_set_flip_vertically_on_load(1);

	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	/* Loads the Texture Data into the Local Buffer */
	m_LocalBuffer = stbi_load(a_path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	if (m_LocalBuffer)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		std::cout << "Failed to Load Data" << std::endl;
	}

	stbi_image_free(m_LocalBuffer);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_RendererID);
}

void Texture::Bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::UnBind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
