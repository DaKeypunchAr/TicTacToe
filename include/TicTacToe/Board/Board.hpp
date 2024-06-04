#ifndef _BOARD_HPP
#define _BOARD_HPP

#include <iostream>
#include <array>

#include "UI/Button/Button.hpp"

namespace TTT
{
	class Board : ButtonHandle
	{
	public:
		Board() = default;
		void initialize(std::array<glm::ivec2, 3>& boardBounds);

		bool isValidMove(unsigned char index) const;
		void checkDraw();
		void checkPlayerWin(unsigned char player);

		unsigned char getIndex(unsigned int i) const { return m_Board[i]; }
		unsigned char getCurrentPlayer() const { return m_CurrentPlayer; }

		void placeMove(unsigned char index);

		void reset();

		bool isGameComplete() const { return m_BoardState != PLAYING; }
		bool isDraw() const { return m_BoardState == DRAW; }
		bool isPlayer1Win() const { return m_BoardState == PLAYER1_WIN; }
		bool isPlayer2Win() const { return m_BoardState == PLAYER2_WIN; }

		int pollHumanEvent();

		void render(const glm::mat4& model = glm::mat4(1.0f));

		void onHover(bool hover, glm::ivec2 mousePos, const std::string& tag) override;
		void onClick(bool released, glm::ivec2 mousePos, int buttonState, const std::string& tag) override;

	public:
		static const unsigned char PLAYING = 0;
		static const unsigned char PLAYER1_WIN = 1;
		static const unsigned char PLAYER2_WIN = 2;
		static const unsigned char DRAW = 3;

	private:
		unsigned char m_BoardState = PLAYING;
		unsigned char m_CurrentPlayer = 1;
		unsigned char m_Board[9]{ 0, 0, 0, 0, 0, 0, 0, 0, 0 };

		std::array<Button, 9> m_Buttons;
		unsigned char m_HumanAction = 9;
	};
}

#endif