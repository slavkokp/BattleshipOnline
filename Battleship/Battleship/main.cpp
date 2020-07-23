#include "pch.h"
#include "Controllers/Game.h"

int main()
{
	unsigned int time_ui = static_cast<unsigned int>(time(NULL));
	srand(time_ui);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	Battleship::Game game(800, 400);
	game.run();
	return 0;
}