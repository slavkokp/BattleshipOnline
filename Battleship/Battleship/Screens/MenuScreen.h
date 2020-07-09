#pragma once

#include "MapEditingScreen.h"

namespace Battleship
{
	class MenuScreen : public Screen
	{
	private:
		GameData* data;

		sf::CircleShape testShape;
		sf::Sprite background;

		std::map<std::string, Button*> menuOptions;

		sf::Vector2f mousePosView;
	public:
		MenuScreen(GameData* data);
		~MenuScreen();

		void updateMenuOptions();
		void updateMousePosView();
		void update()override;
		void handleEvents()override;
		void render()override;
	};
}