#ifndef _PLAYER_HPP
#define _PLAYER_HPP

#include "../Board/Board.hpp"

namespace TTT
{
	typedef int (*PlayerActionFun)(Board& board, unsigned char player, void* param);
	class Player
	{
	public:
		Player(unsigned char playerID, PlayerActionFun fun) : m_Player(playerID), m_Action(fun) { m_Initialized = true; }
		Player() = default;

		int getNextMove(Board& b) const
		{
			if (!m_Initialized) return -2;
			return m_Action(b, m_Player, nullptr);
		}

		unsigned char getPlayer() const
		{
			if (!m_Initialized)
				return 254;
			return m_Player;
		}

		void initialize(unsigned char playerID, PlayerActionFun fun)
		{
			if (m_Initialized) return;
			m_Initialized = true;
			m_Player = playerID;
			m_Action = fun;
		}

	protected:
		bool m_Initialized = false;
		unsigned char m_Player;
		PlayerActionFun m_Action;
	};
}

#endif