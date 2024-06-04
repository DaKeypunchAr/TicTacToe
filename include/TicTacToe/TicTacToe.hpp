#ifndef _TICTACTOE_HPP
#define _TICTACTOE_HPP

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "GL/glew.h"

#include "ft2build.h"
#include FT_FREETYPE_H

#include "ShaderProgram/Program.hpp"
#include "Texture/Texture.hpp"
#include "Board/Board.hpp"
#include "Player/Human.hpp"
#include "UI/Button/Button.hpp"
#include "TextRenderer/TextRenderer.hpp"

#include "root.hpp"

void _GLFWDebugCallback(int error_code, const char* description);
void __stdcall _GLDebugCallback
	(
		unsigned int source,
		unsigned int type,
		unsigned int id,
		unsigned int severity,
		int length,
		const char *message,
		const void *userParameter
	);

namespace TTT
{
	enum class GameState
	{
		PLAY,
		PLAYER1_WIN,
		PLAYER2_WIN,
		DRAW,
		MENU,
	};

	class Game : ButtonHandle
	{
	public:
		Game();
		~Game();

		void tick();

		bool shouldClose() const noexcept
		{
			return glfwWindowShouldClose(m_Window);
		}

	private:

		void render();
		void update();

		void updatePieceBuffer(unsigned char player, unsigned char index);
		void resetBoard();

		void onHover(bool hover, glm::ivec2 mousePos, const std::string& tag) override;
		void onClick(bool released, glm::ivec2 mousePos, int buttonState, const std::string& tag) override;

	private:
		/* Window */
		GLFWwindow* m_Window;

		/* Game Details and Globals */
		static const constexpr unsigned int boxSize = 284;
		static const constexpr unsigned int barWidth = 24;
		static const constexpr unsigned int innerGap = 20;

		static const constexpr unsigned int winSize = 3 * boxSize + 2 * barWidth;

		glm::mat4 m_ProjectionMatrix = glm::ortho(0.0f, (float)winSize, 0.0f, (float)winSize);
		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
		const MVP m_NoModel = { m_ProjectionMatrix, m_ViewMatrix };

		/* Board */
		unsigned int m_BoardVAO, m_BoardVBO, m_BoardEBO, m_BoardEBElements = 24;
		MVP m_BoardMVP = { m_ProjectionMatrix, m_ViewMatrix };

		unsigned int m_PieceVAO, m_PiecePositionVBO, m_PieceTextureCoordsVBO, m_PieceEBO, m_PieceEBElements = 54;
		glm::vec2 oTR, oBL, xTR, xBL;

		/* Game */
		Board m_Board;
		HumanPlayer p1, p2;

		glm::ivec2 bound1, bound2, bound3;

		GameState state = GameState::MENU;

		/* UI */
		float m_ButtonScale = 56.0f;
		TextRenderer tRenderer;
		Button m_PlayButton;
		MVP m_PlayButtonMVP;
		unsigned int m_ButtonVAO, m_ButtonVBO, m_ButtonEBO;

		/* Shaders */
		OGL::Program m_ColorProgram;
		OGL::Program m_TextProgram;

		/* Font Stuff */
		FT_Library m_FtLib;
		FT_Face m_PieceFont;
		const unsigned char m_PieceFontSize = 255;
		OGL::Texture2D m_PieceTexture;
	};
}

#endif