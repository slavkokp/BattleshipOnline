#pragma once

#include <cctype>
#include <sstream>
#include "../Network/Connection.h"
#include "../Screens/MenuScreen.h"

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