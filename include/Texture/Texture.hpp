#ifndef _TEXTURE_HPP
#define _TEXTURE_HPP

#include "GL/glew.h"
#include "glm/glm.hpp"

namespace OGL
{
	struct TexParameters
	{
		unsigned int mipmapLevels = 1;
		unsigned int internalFormat = GL_RGBA;
		unsigned int minFilter = GL_LINEAR;
		unsigned int magFilter = GL_LINEAR;
		unsigned int wrapS = GL_REPEAT;
		unsigned int wrapT = GL_REPEAT;

		TexParameters(unsigned int mipmapLevels = 1,
			unsigned int internalFormat = GL_RGBA,
			unsigned int minFilter = GL_LINEAR,
			unsigned int magFilter = GL_LINEAR,
			unsigned int wrapS = GL_REPEAT,
			unsigned int wrapT = GL_REPEAT)
			: mipmapLevels(mipmapLevels), internalFormat(internalFormat), minFilter(minFilter), magFilter(magFilter), wrapS(wrapS), wrapT(wrapT) {}
	};

	class Texture2D
	{
	public:
		Texture2D() : m_ID(), m_TextureDim(0), m_Params() {}
		~Texture2D();

		void bind(unsigned int slot) const;
		void subImage(unsigned int level, glm::uvec2 offset, glm::uvec2 dim, unsigned char* img);

		void initialize(glm::uvec2 textureDim, TexParameters params = {});

		glm::uvec2 getDim() const noexcept { return m_TextureDim; }
		unsigned int getWidth() const noexcept { return m_TextureDim.x; }
		unsigned int getHeight() const noexcept { return m_TextureDim.y; }

	private:
		unsigned int m_ID;
		glm::uvec2 m_TextureDim;
		TexParameters m_Params;
	};
}

#endif