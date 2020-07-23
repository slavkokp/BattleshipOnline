#pragma once

#include "GameScreen.h"

namespace Battleship
{
	class MapEditingScreen : public Screen
	{
	private:
		Screen* nextScreenPtr;

		GameData* data;
		MapShape* map;
		sf::Vector2f cellSize;
		std::map<std::string, Button*> buttons;
		sf::Sprite backgroundSprite;

		sf::Text message;
		bool displayMessage;
	public:
		MapEditingScreen(GameData* data, Screen* nextScreenPtr);
		~MapEditingScreen();

		void initBackgroundSprite();
		void initMessage();
		void initButtons();

		// applies changes on map to actual player's map
		bool saveMap();
		void setShipsPositionToDefault();
		void updateButtons();
		void updateShipsDragging();
		void update()override;
		void handleEvents()override;
		void render()override;
	};
}