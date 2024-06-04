#include "TicTacToe/Board/Board.hpp"

namespace TTT
{
	void Board::initialize(std::array<glm::ivec2, 3>& boardBounds)
	{
		GLFWwindow* window = glfwGetCurrentContext();
		for (unsigned int i = 0; i < 3; i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				unsigned int idx = i * 3 + j;

				glm::ivec2 bl = glm::ivec2(boardBounds[j].x, boardBounds[2 - i].x);
				glm::ivec2 tr = glm::ivec2(boardBounds[j].y, boardBounds[2 - i].y);
				m_Buttons[i * 3 + j].initialize(bl, tr, this, window, std::to_string(i * 3 + j));
			}
		}
	}

	bool Board::isValidMove(unsigned char index) const
	{
		if (index > 8) return false;
		return m_Board[index] == 0;
	}

	void Board::checkDraw()
	{
		if (m_Board[0] != 0
			&& m_Board[1] != 0
			&& m_Board[2] != 0
			&& m_Board[3] != 0
			&& m_Board[4] != 0
			&& m_Board[5] != 0
			&& m_Board[6] != 0
			&& m_Board[7] != 0
			&& m_Board[8] != 0
			&& m_BoardState == PLAYING)
		{
			m_BoardState = DRAW;
		}
	}

	void Board::checkPlayerWin(unsigned char player)
	{
		if (((m_Board[0] == player && m_Board[1] == player && m_Board[2] == player)
			|| (m_Board[3] == player && m_Board[4] == player && m_Board[5] == player)
			|| (m_Board[6] == player && m_Board[7] == player && m_Board[8] == player)
			|| (m_Board[0] == player && m_Board[3] == player && m_Board[6] == player)
			|| (m_Board[1] == player && m_Board[4] == player && m_Board[7] == player)
			|| (m_Board[2] == player && m_Board[5] == player && m_Board[8] == player)
			|| (m_Board[0] == player && m_Board[4] == player && m_Board[8] == player)
			|| (m_Board[2] == player && m_Board[4] == player && m_Board[6] == player))
			&& m_BoardState == PLAYING)
		{
			m_BoardState = (player == 1) ? PLAYER1_WIN : PLAYER2_WIN;
		}
	}

	void Board::placeMove(unsigned char index)
	{
		m_Board[index] = m_CurrentPlayer;
		checkPlayerWin(m_CurrentPlayer);
		checkDraw();
		m_CurrentPlayer = (m_CurrentPlayer == 1) ? 2 : 1;
	}

	void Board::reset()
	{
		for (unsigned char i = 0; i < 9; i++)
			m_Board[i] = 0;
		m_BoardState = PLAYING;
		m_CurrentPlayer = 1;
	}

	int Board::pollHumanEvent()
	{
		for (Button& button : m_Buttons)
			button.pollEvents();

		if (m_HumanAction == 9) return -1;

		unsigned char index = m_HumanAction;
		m_HumanAction = 9;
		return index;
	}

	void Board::onClick(bool released, glm::ivec2 mousePos, int buttonState, const std::string& tag)
	{
		if (!(!released && buttonState == -1)) return;
		unsigned char index = tag.at(0) - '0';
		if (!isValidMove(index)) return;
		m_HumanAction = index;
	}

	void Board::onHover(bool hover, glm::ivec2 mousePos, const std::string& tag)
	{

	}

	void Board::render(const glm::mat4& model)
	{

	}
}