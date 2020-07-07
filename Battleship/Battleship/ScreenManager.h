#pragma once

#ifndef SFML_GRAPHICS_WINDOW
#define SFML_GRAPHICS_WINDOW

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#endif

#include <stack>
#include "Screen.h"

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
	};
}


