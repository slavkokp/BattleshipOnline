#pragma once

#include <stack>
#include "ResourceManager.h"
#include "../Screens/Screen.h"
#include "../Widgets/Button.h"
#include "../GameClasses/Player.h"
#include "../Controllers/InputManager.h"

namespace Battleship
{
	class ScreenManager
	{
	private:
		std::stack<Screen*> screens;
		Screen* newScreenPtr;

		bool replace;
		bool remove;
		bool add;

	public:
		ScreenManager();
		~ScreenManager();

		void handleScreenSwitches();
		void removeScreen();
		void addScreen(Screen* screen, bool replaceCurrent);
		Screen& getCurrentScreen();
	};
	struct GameData
	{
		Player player;
		sf::RenderWindow window;
		ScreenManager screenManager;
		ResourceManager resourceManager;
		InputManager inputManager;
	};
}


