#include "TextRenderer/TextRenderer.hpp"

#include "root.hpp"

namespace TTT
{
	TextRenderer::~TextRenderer()
	{
		glDeleteBuffers(1, &m_PosVBO);
		glDeleteBuffers(1, &m_TexVBO);
		glDeleteBuffers(1, &m_EBO);
		glDeleteVertexArrays(1, &m_VAO);
	}

	void TextRenderer::initialize(const std::string& font)
	{
		FT_Library ft; FT_Init_FreeType(&ft);
		FT_Face fontFace;
		FT_New_Face(ft, font.c_str(), 0, &fontFace);

		FT_Set_Pixel_Sizes(fontFace, 0, 128);

		glm::uvec2 texDim(0);

		unsigned int glyphGap = 10;

		FT_GlyphSlot g = fontFace->glyph;

		for (char c = 32; c < 127; c++)
		{
			if (!FT_Load_Char(fontFace, c, FT_LOAD_RENDER))
			{
				texDim.x += g->bitmap.width + glyphGap;
				texDim.y = (g->bitmap.rows > texDim.y) ? g->bitmap.rows : texDim.y;
			}
		}

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		m_Atlas.initialize(texDim, OGL::TexParameters(1, GL_R8));

		int offset = 0;
		for (char c = 32; c < 127; c++)
		{
			if (!FT_Load_Char(fontFace, c, FT_LOAD_RENDER))
			{
				unsigned char idx = c - 32;
				m_CharInfos[idx].advance = { (float)g->advance.x / 64.0f, (float)g->advance.y / 64.0f };
				m_CharInfos[idx].bearing = { g->bitmap_left, g->bitmap_top };
				m_CharInfos[idx].dim = { g->bitmap.width, g->bitmap.rows };
				m_CharInfos[idx].texCoordsBL = glm::vec2(offset, 0) / glm::vec2(texDim);
				m_CharInfos[idx].texCoordsTR = m_CharInfos[idx].texCoordsBL + m_CharInfos[idx].dim / glm::vec2(texDim);

				m_Atlas.subImage(0, glm::vec2(offset, 0), m_CharInfos[idx].dim, g->bitmap.buffer);
				offset += static_cast<int>(m_CharInfos[idx].dim.x) + glyphGap;
			}
		}

		FT_Done_Face(fontFace);
		FT_Done_FreeType(ft);

		glCreateVertexArrays(1, &m_VAO);

		glCreateBuffers(1, &m_EBO);
		glVertexArrayElementBuffer(m_VAO, m_EBO);

		glCreateBuffers(1, &m_PosVBO);
		glVertexArrayVertexBuffer(m_VAO, 0, m_PosVBO, 0, sizeof(float) * 2);
		glVertexArrayAttribFormat(m_VAO, 0, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(m_VAO, 0, 0);
		glEnableVertexArrayAttrib(m_VAO, 0);

		glCreateBuffers(1, &m_TexVBO);
		glVertexArrayVertexBuffer(m_VAO, 1, m_TexVBO, 0, sizeof(float) * 2);
		glVertexArrayAttribFormat(m_VAO, 1, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(m_VAO, 1, 1);
		glEnableVertexArrayAttrib(m_VAO, 1);
	}

	void TextRenderer::renderText(const std::string& text, glm::vec2 pos, const OGL::Program& program, const MVP& mvp, Color4 color, float scale) const
	{
		if (text.length() == 0) return;
		if (scale == 0) return;
		if (text.length() > m_TextLimit)
		{
			unsigned int* eb = (unsigned int*)_alloca(sizeof(unsigned int) * 2 * 6 * text.length());
			for (unsigned int i = 0; i < text.length(); i++)
			{
				*(eb + i * 6) = i * 4;
				*(eb + i * 6 + 1) = i * 4 + 1;
				*(eb + i * 6 + 2) = i * 4 + 2;
				*(eb + i * 6 + 3) = i * 4 + 1;
				*(eb + i * 6 + 4) = i * 4 + 2;
				*(eb + i * 6 + 5) = i * 4 + 3;
			}

			glNamedBufferData(m_EBO, sizeof(unsigned int) * 8 * text.length(), eb, GL_STATIC_DRAW);
			glNamedBufferData(m_PosVBO, sizeof(float) * 2 * 4 * text.length(), nullptr, GL_DYNAMIC_DRAW);
			glNamedBufferData(m_TexVBO, sizeof(float) * 2 * 4 * text.length(), nullptr, GL_DYNAMIC_DRAW);
			m_TextLimit = text.length();
		}

		float* posVB = (float*)_alloca(sizeof(float) * 8 * text.length());
		float* texVB = (float*)_alloca(sizeof(float) * 8 * text.length());

		glm::vec2 br = pos;
		for (unsigned int i = 0; i < text.length(); i++)
		{
			unsigned char idx = text.at(i) - 32;
			CharInfo in = m_CharInfos[idx];

			float pvb[8]
			{
				br.x + in.bearing.x * scale / 128.0f, br.y + in.bearing.y * scale / 128.0f,
				br.x + (in.bearing.x + in.dim.x) * scale / 128.0f, br.y + in.bearing.y * scale / 128.0f,
				br.x + in.bearing.x * scale / 128.0f, br.y + (in.bearing.y - in.dim.y) * scale / 128.0f,
				br.x + (in.bearing.x + in.dim.x) * scale / 128.0f, br.y + (in.bearing.y - in.dim.y) * scale / 128.0f
			};
			float tvb[8]
			{
				in.texCoordsBL.x, in.texCoordsBL.y,
				in.texCoordsTR.x, in.texCoordsBL.y,
				in.texCoordsBL.x, in.texCoordsTR.y,
				in.texCoordsTR.x, in.texCoordsTR.y
			};

			memcpy(posVB + i * 8, pvb, sizeof(float) * 8);
			memcpy(texVB + i * 8, tvb, sizeof(float) * 8);

			br += in.advance * scale / 128.0f;
		}

		glNamedBufferSubData(m_PosVBO, 0, text.length() * 8 * sizeof(float), posVB);
		glNamedBufferSubData(m_TexVBO, 0, text.length() * 8 * sizeof(float), texVB);

		program.use();

		m_Atlas.bind(0);
		program.uni1i("uTexture", 0);

		program.uniMVP(mvp);
		program.uniColor(color);
		glBindVertexArray(m_VAO);
		glDrawElements(GL_TRIANGLES, text.length() * 6, GL_UNSIGNED_INT, nullptr);
	}
	void TextRenderer::renderText(const std::string& text, glm::vec2 pos, const OGL::Program& program, const MVP& mvp, Color3 color, float scale) const
	{
		renderText(text, pos, program, mvp, Color4(color, 1.0F), scale);
	}
	void TextRenderer::renderTextCentered(const std::string& text, glm::vec2 pos, const OGL::Program& program, const MVP& mvp, Color4 color, float scale) const
	{
		glm::vec2 centeredPos = pos - getTextDimensions(text, scale) / 2.0f;
		renderText(text, centeredPos, program, mvp, color, scale);
	}
	void TextRenderer::renderTextCentered(const std::string& text, glm::vec2 pos, const OGL::Program& program, const MVP& mvp, Color3 color, float scale) const
	{
		renderTextCentered(text, pos, program, mvp, Color4(color, 1.0F), scale);
	}

	glm::vec2 TextRenderer::getTextDimensions(const std::string& text, float scale) const
	{
		glm::vec2 dimensions{};

		for (unsigned int i = 0; i < text.length(); i++)
		{
			unsigned char idx = text.at(i) - 32;
			dimensions.x += ((i+1) < text.length()) ? m_CharInfos[idx].advance.x : m_CharInfos[idx].dim.x;
			dimensions.y = (m_CharInfos[idx].dim.y > dimensions.y) ? m_CharInfos[idx].dim.y : dimensions.y;
		}
		return dimensions * scale / 128.0f;
	}
}