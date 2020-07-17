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

		bool areShipsInDefaultPos;
		bool empty;

		// static constants

		static const unsigned int CELLS_COUNT = 100;
		static const unsigned int CELLS_IN_ROW_COUNT = 10;
		static const unsigned int SHIPS_COUNT = 10;

		// private methods

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		
		void initCells(sf::Color cellFillColor, sf::Color cellOutlineColor);
		void initShips();
	public:
		MapShape(BattleshipMap& battleshipMap, sf::Vector2f cellSize, bool empty, sf::Vector2f position = sf::Vector2f(0.f, 0.f),
				sf::Color cellFillColor = sf::Color::Blue, sf::Color cellOutlineColor = sf::Color::Black);
		~MapShape();

		void saveMapShipsLocations();
		void setShipsPositionToDefault();
		void updateShipsDragging();
		void updateCellChoosing(const sf::Vector2f& mousePos);
		
		// setters

		void setPosition(const sf::Vector2f& position);
		void setPosition(float x, float y);

		// getters

		const sf::Vector2f& getSize() const;
		const sf::Vector2f& getPosition() const;
	};
}