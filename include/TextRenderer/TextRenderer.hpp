#ifndef _TEXT_RENDERER_HPP
#define _TEXT_RENDERER_HPP

#include <string>

#include "ft2build.h"
#include FT_FREETYPE_H

#include "Texture/Texture.hpp"
#include "ShaderProgram/Program.hpp"
#include <array>

namespace TTT
{
	struct CharInfo
	{
		glm::vec2 texCoordsBL, texCoordsTR, bearing, advance, dim;
	};

	class TextRenderer
	{
		public:
			TextRenderer() 
				: m_Atlas(), m_CharInfos(), m_VAO(), m_PosVBO(), m_TexVBO(), m_EBO() {}
			TextRenderer(const std::string& font)
				: m_Atlas(), m_CharInfos()
			{
				initialize(font);
			}
			~TextRenderer();

			void initialize(const std::string& font);

			void renderText(const std::string& text, glm::vec2 pos, const OGL::Program& program, const MVP& mvp, Color3 color = BLACK, float scale = 32.0f) const;
			void renderText(const std::string& text, glm::vec2 pos, const OGL::Program& program, const MVP& mvp, Color4 color = Color4(BLACK, 1.0F), float scale = 32.0f) const;
			void renderTextCentered(const std::string& text, glm::vec2 pos, const OGL::Program& program, const MVP& mvp, Color3 color = BLACK, float scale = 32.0f) const;
			void renderTextCentered(const std::string& text, glm::vec2 pos, const OGL::Program& program, const MVP& mvp, Color4 color = Color4(BLACK, 1.0F), float scale = 32.0f) const;
			glm::vec2 getTextDimensions(const std::string& text, float scale = 32.0f) const;

		private:
			unsigned int m_VAO, m_PosVBO, m_TexVBO, m_EBO;
			mutable unsigned int m_TextLimit = 0;

			OGL::Texture2D m_Atlas;
			std::array<CharInfo, 95> m_CharInfos;
	};
}

#endif