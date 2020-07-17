#pragma once

#include "GameScreen.h"

namespace Battleship
{
	class MapEditingScreen : public Screen
	{
	private:
		bool callingByPlayButton;

		GameData* data;
		sf::RectangleShape map;
		sf::RectangleShape* cells;
		sf::RectangleShape* ships;
		sf::Vector2f shipsStartingPos[4];
		sf::Vector2f cellSize;
		std::map<std::string, Button*> buttons;
		sf::Sprite backgroundSprite;

		sf::Text message;
		bool displayMessage;

		bool areShipsInDefaultPos;
		bool multipleSaveCallsProtection;
		bool dragStart;
		bool* dragShips;
		int dragShipIndex;
		sf::Vector2f posDiff;
		sf::Vector2f startPosBackup;
		bool rotated;
		bool returnToDefaultPos;
	public:
		MapEditingScreen(GameData* data, bool callingByPlayButton);
		~MapEditingScreen();

		void initBackgroundSprite();
		void initMessage();
		void initButtons();
		void initMap();
		void initShips();
		void initCells();
		void initDragShips();

		// applies changes on map to actual players map
		bool saveMap();
		void setShipsPositionToDefault();
		void updateButtons();
		void updateShipsDragging();
		void update()override;
		void handleEvents()override;
		void render()override;
	};
}