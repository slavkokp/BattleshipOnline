#pragma once

#include "ScreenManager.h"

namespace Battleship
{
	class MapEditingScreen : public Screen
	{
	private:
		GameData* data;
		sf::RectangleShape test;
	public:
		MapEditingScreen(GameData* data);
		~MapEditingScreen();

		void update()override;
		void handleEvents()override;
		void render()override;
	};
}