#pragma once

#include <stack>
#include <SFML/Window.hpp>
#include "Screen.h"
#include "ResourceManager.h"

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
		sf::RenderWindow window;
		ScreenManager screenManager;
		ResourceManager resourceManager;
	};
}


