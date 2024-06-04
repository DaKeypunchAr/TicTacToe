#include "TicTacToe/TicTacToe.hpp"

int main()
{
	TTT::Game game;
	double frameTime = 1.0 / 60;
	double lastTime = glfwGetTime();

	while (!game.shouldClose())
	{
		if (lastTime + frameTime < glfwGetTime())
		{
			game.tick();
			lastTime = glfwGetTime();
		}
	}
}