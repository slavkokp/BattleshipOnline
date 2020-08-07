#pragma once

#include "MapEditingScreen.h"
#include "HostScreen.h"
#include "MatchHistoryScreen.h"

namespace Battleship
{
	class MenuScreen : public Screen
	{
	private:
		GameData* data;

		sf::CircleShape testShape;
		sf::Sprite background;

		std::map<std::string, Button*> menuOptions;
	public:
		MenuScreen(GameData* data);
		~MenuScreen();

		void updateMenuOptionsVisual();
		void updateMenuOptionsFunction();
		void update()override;
		void handleEvents()override;
		void render()override;
	};
}