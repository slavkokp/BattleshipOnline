#pragma once

#include <cctype>
#include <sstream>
#include "../Screens/LoginScreen.h"

namespace Battleship
{
	class Game
	{
	private:
		GameData* gameData;
	public:
		Game(int windowWidth, int windowHeight);
		~Game();

		void run();
	};
}