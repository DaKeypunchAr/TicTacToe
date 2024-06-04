#include "TicTacToe/TicTacToe.hpp"

#include <iostream>

void _GLFWDebugCallback(int error_code, const char *description)
{
#ifdef _DEBUG
	__debugbreak();
#endif
	std::cout << "GLFW Debug Callback:\n\tError Code: " << error_code << "\n\tDescription: " << description << '\n';
}
void __stdcall _GLDebugCallback
	(
		unsigned int source,
		unsigned int type,
		unsigned int id,
		unsigned int severity,
		int length,
		const char* message,
		const void* userParameter
	)
{

	if (_DEBUG)
	{
		__debugbreak();
	}

	std::cout << "GL Callback:\n\tSource: " << source << "\n\tType: " << type << "\n\tID: "
		<< id << "\n\tSeverity: " << severity << "\n\tLength: " << length << "\n\tMessage: " << message << '\n';
}

namespace TTT
{
	Game::Game()
	{
#pragma region Initializing OpenGL Context

#	pragma region Initializing GLFW

		if (!glfwInit())
		{
			exit(EXIT_FAILURE);
		}
		glfwSetErrorCallback(_GLFWDebugCallback);

		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

#	pragma endregion

#	pragma region Initializing OpenGL Window

		m_Window = glfwCreateWindow(winSize, winSize, "Tic Tac Toe", nullptr, nullptr);

		if (!m_Window)
		{
			glfwTerminate();
			exit(EXIT_FAILURE);
		}

		glfwMakeContextCurrent(m_Window);

#	pragma endregion

#	pragma region Initializing OpenGL using GLEW

		if (glewInit())
		{
			glfwDestroyWindow(m_Window);
			glfwTerminate();
			exit(EXIT_FAILURE);
		}
		glEnable(GL_DEBUG_CALLBACK_FUNCTION);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDebugMessageCallback(_GLDebugCallback, nullptr);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

#	pragma endregion

#pragma endregion

#pragma region Initializing TicTacToe Board

		int p0 = 0, p1 = boxSize, p2 = p1 + barWidth, p3 = p2 + boxSize, p4 = p3 + barWidth, p5 = winSize;

		bound1 = { p0, p1 };
		bound2 = { p2, p3 };
		bound3 = { p4, p5 };

		std::array<glm::ivec2, 3> bounds = { bound1, bound2, bound3 };
		m_Board.initialize(bounds);

		this->p1.initialize(1);
		this->p2.initialize(2);

		float boardVB[32]
		{
			(float)p1, (float)p5,
			(float)p2, (float)p5,
			(float)p3, (float)p5,
			(float)p4, (float)p5,
			(float)p5, (float)p4,
			(float)p5, (float)p3,
			(float)p5, (float)p2,
			(float)p5, (float)p1,
			(float)p4, (float)p0,
			(float)p3, (float)p0,
			(float)p2, (float)p0,
			(float)p1, (float)p0,
			(float)p0, (float)p1,
			(float)p0, (float)p2,
			(float)p0, (float)p3,
			(float)p0, (float)p4,
		};

		p0 += innerGap;
		p1 -= innerGap;
		p2 += innerGap;
		p3 -= innerGap;
		p4 += innerGap;
		p5 -= innerGap;

		unsigned char boardEB[24]
		{
			0, 1, 10,
			0, 10, 11,
			2, 3, 8,
			2, 8, 9,
			15, 4, 5,
			15, 5, 14,
			13, 6, 7,
			13, 7, 12,
		};

		glCreateVertexArrays(1, &m_BoardVAO);

		glCreateBuffers(1, &m_BoardEBO);
		glNamedBufferData(m_BoardEBO, sizeof(unsigned char) * m_BoardEBElements, boardEB, GL_STATIC_DRAW);
		glVertexArrayElementBuffer(m_BoardVAO, m_BoardEBO);

		glCreateBuffers(1, &m_BoardVBO);
		glNamedBufferData(m_BoardVBO, sizeof(float) * 2 * 16, boardVB, GL_STATIC_DRAW);
		glVertexArrayVertexBuffer(m_BoardVAO, 0, m_BoardVBO, 0, 2 * sizeof(float));
		glVertexArrayAttribFormat(m_BoardVAO, 0, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(m_BoardVAO, 0, 0);
		glEnableVertexArrayAttrib(m_BoardVAO, 0);

#pragma endregion

#pragma region Initializing TicTacToe Pieces

		glCreateVertexArrays(1, &m_PieceVAO);

		unsigned char pieceEB[54]
		{
			0, 1, 2,  1, 2, 3,
			4, 5, 6,  5, 6, 7,
			8, 9, 10,  9, 10, 11,
			12, 13, 14,  13, 14, 15,
			16, 17, 18,  17, 18, 19,
			20, 21, 22,  21, 22, 23,
			24, 25, 26,  25, 26, 27,
			28, 29, 30,  29, 30, 31,
			32, 33, 34,  33, 34, 35
		};

		float piecePosVB[72]
		{
			(float)p0, (float)p5,
			(float)p1, (float)p5,
			(float)p0, (float)p4,
			(float)p1, (float)p4,

			(float)p2, (float)p5,
			(float)p3, (float)p5,
			(float)p2, (float)p4,
			(float)p3, (float)p4,

			(float)p4, (float)p5,
			(float)p5, (float)p5,
			(float)p4, (float)p4,
			(float)p5, (float)p4,

			(float)p0, (float)p3,
			(float)p1, (float)p3,
			(float)p0, (float)p2,
			(float)p1, (float)p2,

			(float)p2, (float)p3,
			(float)p3, (float)p3,
			(float)p2, (float)p2,
			(float)p3, (float)p2,

			(float)p4, (float)p3,
			(float)p5, (float)p3,
			(float)p4, (float)p2,
			(float)p5, (float)p2,

			(float)p0, (float)p1,
			(float)p1, (float)p1,
			(float)p0, (float)p0,
			(float)p1, (float)p0,

			(float)p2, (float)p1,
			(float)p3, (float)p1,
			(float)p2, (float)p0,
			(float)p3, (float)p0,

			(float)p4, (float)p1,
			(float)p5, (float)p1,
			(float)p4, (float)p0,
			(float)p5, (float)p0,
		};

		float pieceTexCoordsVB[72]
		{
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
		};

		glCreateBuffers(1, &m_PieceEBO);
		glNamedBufferData(m_PieceEBO, sizeof(unsigned char) * m_PieceEBElements, pieceEB, GL_STATIC_DRAW);
		glVertexArrayElementBuffer(m_PieceVAO, m_PieceEBO);

		glCreateBuffers(1, &m_PiecePositionVBO);
		glNamedBufferData(m_PiecePositionVBO, sizeof(float) * 2 * 36, piecePosVB, GL_STATIC_DRAW);
		glVertexArrayVertexBuffer(m_PieceVAO, 0, m_PiecePositionVBO, 0, 2 * sizeof(float));
		glVertexArrayAttribFormat(m_PieceVAO, 0, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(m_PieceVAO, 0, 0);
		glEnableVertexArrayAttrib(m_PieceVAO, 0);

		glCreateBuffers(1, &m_PieceTextureCoordsVBO);
		glNamedBufferData(m_PieceTextureCoordsVBO, sizeof(float) * 2 * 36, pieceTexCoordsVB, GL_DYNAMIC_DRAW);
		glVertexArrayVertexBuffer(m_PieceVAO, 1, m_PieceTextureCoordsVBO, 0, 2 * sizeof(float));
		glVertexArrayAttribFormat(m_PieceVAO, 1, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(m_PieceVAO, 1, 1);
		glEnableVertexArrayAttrib(m_PieceVAO, 1);

#pragma endregion

#pragma region Shader Initializations

		m_ColorProgram.initialize(ROOT_DIR "/src/Resources/ShaderPrograms/Color");
		m_TextProgram.initialize(ROOT_DIR "/src/Resources/ShaderPrograms/Text");

#pragma endregion

#pragma region Initializing Fonts and Related Stuff

		FT_Init_FreeType(&m_FtLib);

		FT_New_Face(m_FtLib, ROOT_DIR "/src/Resources/Fonts/SpaceMono-Regular.ttf", 0, &m_PieceFont);
		FT_Set_Pixel_Sizes(m_PieceFont, 0, m_PieceFontSize);

		FT_GlyphSlot glyphSlot = m_PieceFont->glyph;

		unsigned int tWidth, tHeight;

		FT_Load_Char(m_PieceFont, 'o', FT_LOAD_RENDER);
		tWidth = glyphSlot->bitmap.width;
		tHeight = glyphSlot->bitmap.rows;

		FT_Load_Char(m_PieceFont, 'x', FT_LOAD_RENDER);
		tWidth += glyphSlot->bitmap.width;
		tHeight = (tHeight > glyphSlot->bitmap.rows) ? tHeight : glyphSlot->bitmap.rows;

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		m_PieceTexture.initialize({ tWidth, tHeight }, OGL::TexParameters(1, GL_R8));

		FT_Load_Char(m_PieceFont, 'o', FT_LOAD_RENDER);
		m_PieceTexture.subImage(0, { 0, 0 }, { glyphSlot->bitmap.width, glyphSlot->bitmap.rows }, glyphSlot->bitmap.buffer);

		oBL = glm::vec2(0, 0);
		oTR = glm::vec2(glyphSlot->bitmap.width, glyphSlot->bitmap.rows) / glm::vec2(tWidth, tHeight);

		FT_Load_Char(m_PieceFont, 'x', FT_LOAD_RENDER);
		m_PieceTexture.subImage(0, { tWidth - glyphSlot->bitmap.width, 0 }, { glyphSlot->bitmap.width, glyphSlot->bitmap.rows }, glyphSlot->bitmap.buffer);

		xBL = glm::vec2(tWidth - glyphSlot->bitmap.width, 0) / glm::vec2(tWidth, tHeight);
		xTR = glm::vec2(glyphSlot->bitmap.width, glyphSlot->bitmap.rows) / glm::vec2(tWidth, tHeight);
		xTR.x += oTR.x;

#pragma endregion

#pragma region Initializing UI Stuff

		tRenderer.initialize(ROOT_DIR "/src/Resources/Fonts/SpaceMono-Regular.ttf");

		glCreateVertexArrays(1, &m_ButtonVAO);
		
		glCreateBuffers(1, &m_ButtonEBO);
		unsigned char bEB[6]
		{
			0, 1, 2,  1, 2, 3,
		};
		glNamedBufferData(m_ButtonEBO, sizeof(unsigned char) * 6, bEB, GL_STATIC_DRAW);
		glVertexArrayElementBuffer(m_ButtonVAO, m_ButtonEBO);

		glCreateBuffers(1, &m_ButtonVBO);
		float bVB[8]
		{
			0, 0,
			0, 1,
			1, 0,
			1, 1,
		};
		glNamedBufferData(m_ButtonVBO, sizeof(float) * 8, bVB, GL_STATIC_DRAW);
		glVertexArrayVertexBuffer(m_ButtonVAO, 0, m_ButtonVBO, 0, sizeof(float) * 2);
		glVertexArrayAttribFormat(m_ButtonVAO, 0, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(m_ButtonVAO, 0, 0);
		glEnableVertexArrayAttrib(m_ButtonVAO, 0);

		m_PlayButtonMVP = { m_ProjectionMatrix, m_ViewMatrix };

		glm::vec2 playDim = tRenderer.getTextDimensions("PLAY", m_ButtonScale) + glm::vec2(2, 1) * m_ButtonScale;
		glm::vec2 playBL = glm::vec2((winSize - playDim.x) / 2.0f, 500.0f - playDim.y / 2.0f);

		m_PlayButtonMVP.model = glm::translate(m_PlayButtonMVP.model, glm::vec3(playBL, 0));
		m_PlayButtonMVP.model = glm::scale(m_PlayButtonMVP.model, glm::vec3(playDim, 0.0f));

		m_PlayButton.initialize(glm::ivec2(playBL), glm::ivec2(playBL + playDim), this, m_Window, "play");

#pragma endregion
	}

	Game::~Game()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();

		glDeleteVertexArrays(1, &m_BoardVAO);
		glDeleteBuffers(1, &m_BoardVBO);
		glDeleteBuffers(1, &m_BoardEBO);

		FT_Done_FreeType(m_FtLib);
	}

	/* Render Logic */
	void Game::render()
	{
		glClear(GL_COLOR_BUFFER_BIT);

		switch (state)
		{
		case GameState::PLAY:
			m_ColorProgram.use();

			glBindVertexArray(m_BoardVAO);
			m_ColorProgram.uniMVP(m_BoardMVP);
			m_ColorProgram.uniColor(BLACK);
			glDrawElements(GL_TRIANGLES, m_BoardEBElements, GL_UNSIGNED_BYTE, nullptr);

			m_TextProgram.use();

			m_PieceTexture.bind(0);
			m_TextProgram.uni1i("uTexture", 0);

			glBindVertexArray(m_PieceVAO);
			m_TextProgram.uniMVP(m_BoardMVP);
			m_TextProgram.uniColor(BLACK);
			glDrawElements(GL_TRIANGLES, m_PieceEBElements, GL_UNSIGNED_BYTE, nullptr);
			break;
		case GameState::MENU:
			m_ColorProgram.use();
			m_ColorProgram.uniMVP(m_PlayButtonMVP);
			m_ColorProgram.uniColor(RED);
			glBindVertexArray(m_ButtonVAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, nullptr);

			tRenderer.renderTextCentered("Tic Tac Toe", glm::vec2(winSize/2.0f, winSize - 200), m_TextProgram, m_NoModel, PINK, 96.0f);
			tRenderer.renderTextCentered("PLAY", glm::vec2(winSize / 2.0f, 500.0f), m_TextProgram, m_NoModel, BLACK, m_ButtonScale);
		}

		glfwSwapBuffers(m_Window);
	}
	void Game::updatePieceBuffer(unsigned char player, unsigned char index)
	{
		if (player == 1)
		{
			float buff[8]
			{
				oBL.x, oTR.y,
				oTR.x, oTR.y,
				oBL.x, oBL.y,
				oTR.x, oBL.y,
			};

			glNamedBufferSubData(m_PieceTextureCoordsVBO, index << 5, 8 * sizeof(float), buff);
		}
		else if (player == 2)
		{
			float buff[8]
			{
				xBL.x, xTR.y,
				xTR.x, xTR.y,
				xBL.x, xBL.y,
				xTR.x, xBL.y
			};

			glNamedBufferSubData(m_PieceTextureCoordsVBO, index << 5, 8 * sizeof(float), buff);
		}
		else
		{
			float buff[8]{ 0, 0, 0, 0, 0, 0, 0, 0, };

			glNamedBufferSubData(m_PieceTextureCoordsVBO, index << 5, 8 * sizeof(float), buff);
		}
	}

	/* Game Logic */
	void Game::update()
	{
		if (glfwGetKey(m_Window, GLFW_KEY_R) == GLFW_PRESS)
			resetBoard();

		if (state == GameState::MENU)
		{
			m_PlayButton.pollEvents();

			glfwPollEvents();
			return;
		}

		int index = (m_Board.getCurrentPlayer() == 1) ? p1.getNextMove(m_Board) : p2.getNextMove(m_Board);
		if (index < 0) 
		{
			glfwPollEvents();
			return;
		}
		updatePieceBuffer(m_Board.getCurrentPlayer(), index);
		m_Board.placeMove(index);
		if (m_Board.isGameComplete())
		{
			if (m_Board.isDraw()) state = GameState::DRAW;
			else if (m_Board.isPlayer1Win()) state = GameState::PLAYER1_WIN;
			else state = GameState::PLAYER2_WIN;
		}

		glfwPollEvents();
	}
	void Game::resetBoard()
	{
		for (unsigned char i = 0; i < 9; i++)
			if (!m_Board.isValidMove(i))
				updatePieceBuffer(0, i);
		m_Board.reset();
		state = GameState::PLAY;
	}
	void Game::tick()
	{
		render();
		update();
	}

	void Game::onHover(bool hover, glm::ivec2 mousePos, const std::string& tag)
	{
	}

	void Game::onClick(bool released, glm::ivec2 mousePos, int buttonState, const std::string& tag)
	{
		if (tag == "play")
		{
			if (buttonState == -1 && released == false)
				state = GameState::PLAY;
		}
	}
};