#ifndef _HUMAN_PLAYER_CPP
#define _HUMAN_PLAYER_CPP

#include "TicTacToe/Player/Human.hpp"
#include "GLFW/glfw3.h"

#include <iostream>

namespace TTT
{
	int humanAction(Board& board, unsigned char player, void* params)
	{
		return board.pollHumanEvent();
	}

	void HumanPlayer::initialize(unsigned char playerID)
	{
		if (m_Initialized) return;
		m_Initialized = true;
		m_Player = playerID;
		m_Action = humanAction;
	}

	int HumanPlayer::getNextMove(Board& b) const
	{
		if (!m_Initialized) return -2;
		return m_Action(b, m_Player, nullptr);
	}
}

#endif