#include "Texture.h"

#include "Extra/stb_image/stb_image.h"

Texture::Texture(const std::string a_path)
	: m_RendererID(0), m_FilePath(a_path), m_LocalBuffer(nullptr), 
	  m_Width(0), m_Height(0), m_BPP(0)
{
	/* Flips the Texture Vertically */
	stbi_set_flip_vertically_on_load(1);

	/* Writes the Texture Data (Width, Height, BPP) into the Local Buffer */
	/* m_LocalBuffer holds all of our Pixel Data */
	m_LocalBuffer = stbi_load(a_path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	/* Loads the Texture and assigns it to RendererID */
	glGenTextures(1, &m_RendererID);
	/* Binds the Texture (Since it is a 2D texture we bind it as a 2D texture) */
	glBindTexture(GL_TEXTURE_2D, m_RendererID);

	/* How the Texture will be re-sampled down */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	/* How the Texture will be re-sampled upwards */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	/* Wrap S is Horizontal Wraps */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	/* Wrap T is Vertical Wraps */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
	/* Unbinds the Texture */
	glBindTexture(GL_TEXTURE_2D, 0);

	/* If the Buffer Contains Data */
	if (m_LocalBuffer)
	{
		stbi_image_free(m_LocalBuffer);
	}
	else
	{
		std::cout << "Failed to Load Data" << std::endl;
	}

}

Texture::~Texture()
{
	/* Deletes the Texture from the GPU */
	glDeleteTextures(1, &m_RendererID);
}

void Texture::Bind(unsigned int slot) const
{
	/* Binds a Texture to a specified Texture Slot */
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::UnBind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
