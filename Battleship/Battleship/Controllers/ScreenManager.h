#pragma once

#include "ResourceManager.h"
#include "../Screens/Screen.h"
#include "../Widgets/Button.h"
#include "../GameClasses/Player.h"
#include "../Controllers/InputManager.h"

namespace Battleship
{
	struct ScreenManagerRequest
	{
		Screen* newScreenPtr;
		bool replace;
		bool remove;
		bool add;
		
		ScreenManagerRequest(Screen* newScreenPtr, bool replace, bool remove, bool add) : 
			newScreenPtr(newScreenPtr), replace(replace), remove(remove), add(add) {}
	};
	class ScreenManager
	{
	private:
		std::stack<Screen*> screens;
		std::queue<ScreenManagerRequest> requests;

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


