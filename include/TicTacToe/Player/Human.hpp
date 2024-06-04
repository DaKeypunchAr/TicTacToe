#ifndef _HUMAN_HPP
#define _HUMAN_HPP

#include "Player.hpp"
#include "glm/glm.hpp"
#include <array>

namespace TTT
{
	int humanAction(Board& board, unsigned char player, void* params);

	class HumanPlayer : Player
	{
	public:
		HumanPlayer() : Player() {}
		HumanPlayer(unsigned char player)
			: Player(player, humanAction) {};

		int getNextMove(Board& b) const;
		void initialize(unsigned char playerID);
	};
}

#endif