#include "Texture/Texture.hpp"

namespace OGL
{
	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &m_ID);
	}

	void Texture2D::bind(unsigned int slot) const
	{
		glBindTextureUnit(slot, m_ID);
	}

	void Texture2D::subImage(unsigned int level, glm::uvec2 offset, glm::uvec2 dim, unsigned char* img)
	{
		glTextureSubImage2D(m_ID, level, offset.x, offset.y, dim.x, dim.y, m_Params.internalFormat, GL_UNSIGNED_BYTE, img);
		if (m_Params.mipmapLevels > 1)
			glGenerateTextureMipmap(m_ID);
	}

	void Texture2D::initialize(glm::uvec2 textureDim, TexParameters params)
	{
		m_TextureDim = textureDim;
		m_Params = params;
		glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
		glTextureStorage2D(m_ID, params.mipmapLevels, params.internalFormat, textureDim.x, textureDim.y);

		if (params.internalFormat == GL_R8) m_Params.internalFormat = GL_RED;

		glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, params.minFilter);
		glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, params.magFilter);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, params.wrapS);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, params.wrapT);
	}
}