#pragma once

#include "ShipShape.h"
#include "../GameClasses/BattleshipMap.h"

namespace Battleship
{
	class MapShape : public sf::Drawable, public sf::Transformable
	{
	private:
		BattleshipMap& battleshipMap;
		sf::RectangleShape map;
		sf::RectangleShape* cells;

		ShipShape** ships;
		sf::Vector2f* shipsStartingPos;

		sf::Vector2f cellSize;
		sf::Color defaultCellColor;
		sf::Color chosenCellColor;
		sf::Color destroyedShipCellColor;
		sf::Color missedCellColor;

		bool areShipsInDefaultPos;
		bool empty;

		// static constants

		static const unsigned int CELLS_COUNT = 100;
		static const unsigned int CELLS_IN_ROW_COUNT = 10;
		static const unsigned int SHIPS_COUNT = 10;

		// variables for dragging
		
		bool dragShips[SHIPS_COUNT];
		bool rotate;
		bool startDrag;
		bool returnToDefaultPos;
		int dragShipIndex;
		sf::Vector2f posDiff;
		sf::Vector2f startPosBackup;

		// private methods

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		
		void initCells(sf::Color cellOutlineColor);
		void initShips();
		void initShipsStartingPos(const sf::Vector2f& mapPos);
		void initDraggingVariables();
	public:
		MapShape(BattleshipMap& battleshipMap, sf::Vector2f cellSize, bool empty, sf::Vector2f position = sf::Vector2f(0.f, 0.f),
				sf::Color cellFillColor = sf::Color::Blue, sf::Color cellOutlineColor = sf::Color::Black);
		~MapShape();

		//changes color only
		void destroyShipOnCell(int cellIndex);

		//changes color only
		void missOnCell(int cellIndex);

		//changes color only
		void chooseCell(int cellIndex);

		void rotateCurDraggedShip();
		bool saveMapShipsLocations();
		void setShipsPositionToDefault();
		void updateShipsDragging(sf::Vector2f& mousePosView, bool& displayMessage);
		void updateCellChoosingVisual(const sf::Vector2f& mousePos);
		void updateCellChoosing(const sf::Vector2f& mousePos, int& attackedCellIndex);
		
		// setters

		void setPosition(const sf::Vector2f& position);
		void setPosition(float x, float y);

		// default is sf::Color(0, 0, 139)
		void setChosenCellColor(sf::Color color);

		// default is sf::Color::Red
		void setDestroyedShipCellColor(sf::Color color);

		// default is sf::Color(0, 0, 100)
		void setMissedCellColor(sf::Color color);

		// default is sf::Color::Blue
		void setDefaultCellColor(sf::Color color);

		// getters

		const sf::Vector2f& getSize() const;
		const sf::Vector2f& getPosition() const;
		sf::FloatRect getGlobalBounds() const;
		bool getAreShipsInDefaultPos() const;
		bool getStartDrag() const;
		sf::RectangleShape& operator[](int i);
	};
}