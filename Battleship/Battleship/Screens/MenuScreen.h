#pragma once

#include "MapEditingScreen.h"

namespace Battleship
{
	class MenuScreen : public Screen
	{
	private:
		GameData* data;
		sf::CircleShape testShape;
	public:
		MenuScreen(GameData* data);
		~MenuScreen();

		void update()override;
		void handleEvents()override;
		void render()override;
	};
}