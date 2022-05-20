#include "Game.h"

int main()
{
	Game game;

	if (game.Construct(200, 200, 4, 4, false, true))
		game.Start();

	return 0;
}