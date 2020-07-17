#include "MapShape.h"

namespace Battleship
{
	MapShape::MapShape(BattleshipMap& battleshipMap, sf::Vector2f cellSize, bool empty, sf::Vector2f position, sf::Color cellFillColor, sf::Color cellOutlineColor)
		: cellSize(cellSize), battleshipMap(battleshipMap), empty(empty)
	{
		this->cells = new sf::RectangleShape[CELLS_COUNT];
		this->initCells(cellFillColor, cellOutlineColor);

		if (!this->empty)
		{
			this->ships = new ShipShape*[SHIPS_COUNT];
			this->shipsStartingPos = new sf::Vector2f[4];
			this->initShips();
		}

		this->map.setSize(cellSize * 10.f);
		this->map.setFillColor(sf::Color::Transparent);
	}

	MapShape::~MapShape()
	{
		if (!this->empty)
		{
			for (int i = 0; i < SHIPS_COUNT; i++)
			{
				delete this->ships[i];
			}
			delete[] ships;
		}
		delete[] cells;
		delete[] shipsStartingPos;
	}

	void MapShape::initCells(sf::Color cellFillColor, sf::Color cellOutlineColor)
	{
		sf::Vector2f mapPos = this->map.getPosition();

		for (int i = 0; i < CELLS_IN_ROW_COUNT; i++)
		{
			for (int j = 0; j < CELLS_IN_ROW_COUNT; j++)
			{
				this->cells[i * CELLS_IN_ROW_COUNT + j].setPosition(mapPos.x + i * cellSize.x, mapPos.y + j * cellSize.y);
				this->cells[i * CELLS_IN_ROW_COUNT + j].setSize(cellSize);

				this->cells[i * CELLS_IN_ROW_COUNT + j].setFillColor(cellFillColor);
				this->cells[i * CELLS_IN_ROW_COUNT + j].setOutlineColor(cellOutlineColor);
				this->cells[i * CELLS_IN_ROW_COUNT + j].setOutlineThickness(-1.f);
			}
		}
	}

	void MapShape::initShips()
	{
		sf::Vector2f mapPos = this->map.getPosition();

		// set starting positions of ships
		for (int i = 0; i < 4; i++)
		{
			this->shipsStartingPos[i].x = mapPos.x + i * cellSize.x * 2;
			this->shipsStartingPos[i].y = mapPos.y - cellSize.x * 4;
		}

		// if map is invalid clear it and set ships position to defalut
		// else read ships positions from battleshipMap
		if (!this->battleshipMap.validateMap())
		{
			this->battleshipMap.clear();
			
			for (int i = 1, k = 0; i < 5; i++)
			{
				for (int j = 0; j < 5 - i; j++, k++)
				{
					this->ships[k] = new ShipShape(i, this->cellSize, sf::Color::Green, sf::Color::White, -1.f);
					this->ships[k]->setPosition(shipsStartingPos[i - 1]);
				}
			}
			this->areShipsInDefaultPos = true;
		}
		else
		{
			auto coords = this->battleshipMap.getShipsCoords();
			int iDiff;
			int jDiff;
			for (int i = 0; i < coords.size(); i++)
			{
				iDiff = coords[i].y / 10 - coords[i].x / 10;
				jDiff = coords[i].y % 10 - coords[i].x % 10;
				this->ships[i] = new ShipShape(iDiff + jDiff + 1, this->cellSize, sf::Color::Green, sf::Color::White, -1.f);
				if (jDiff > 0)
				{
					this->ships[i]->rotate();
				}
				this->ships[i]->setPosition(mapPos.x + (coords[i].x % 10) * cellSize.x, mapPos.y + (coords[i].x / 10) * cellSize.y);
				this->areShipsInDefaultPos = false;
			}
		}
	}

	void MapShape::saveMapShipsLocations()
	{
		
	}

	void MapShape::setShipsPositionToDefault()
	{
		for (int i = 1, k = 0; i < 5; i++)
		{
			for (int j = 0; j < 5 - i; j++, k++)
			{
				this->ships[k]->setSize(sf::Vector2f(cellSize.x, cellSize.y * i));
				this->ships[k]->setPosition(shipsStartingPos[i - 1]);
				this->ships[k]->setRotation(0.f);
			}
		}
		this->areShipsInDefaultPos = true;
	}

	void MapShape::updateShipsDragging()
	{

	}

	void MapShape::updateCellChoosing(const sf::Vector2f& mousePos)
	{
		for (int i = 0; i < CELLS_COUNT; i++)
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->cells[i].getGlobalBounds().contains(mousePos))
			{
				this->cells[i].setFillColor(sf::Color::Color(0, 0, 139));
			}
			else if(cells[i].getFillColor() != sf::Color::Blue)
			{
				this->cells[i].setFillColor(sf::Color::Blue);
			}
		}
	}

	void MapShape::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(map, states);
		for (int i = 0; i < CELLS_COUNT; i++)
		{
			target.draw(cells[i]);
		}
		if (!this->empty)
		{
			for (int i = 0; i < SHIPS_COUNT; i++)
			{
				target.draw(*this->ships[i]);
			}
		}
	}

	void MapShape::setPosition(const sf::Vector2f& position)
	{
		this->map.setPosition(position);
		for (int i = 0; i < CELLS_IN_ROW_COUNT; i++)
		{
			for (int j = 0; j < CELLS_IN_ROW_COUNT; j++)
			{
				this->cells[i * CELLS_IN_ROW_COUNT + j].setPosition(position.x + i * cellSize.x, position.y + j * cellSize.y);
			}
		}
		if (!this->empty)
		{
			if (this->areShipsInDefaultPos)
			{
				for (int i = 1, k = 0; i < 5; i++)
				{
					for (int j = 0; j < 5 - i; j++, k++)
					{
						this->ships[k]->setPosition(shipsStartingPos[i - 1]);
					}
				}
			}
			else
			{
				auto coords = this->battleshipMap.getShipsCoords();
				int iDiff;
				int jDiff;
				for (int i = 0; i < coords.size(); i++)
				{
					iDiff = coords[i].y / 10 - coords[i].x / 10;
					jDiff = coords[i].y % 10 - coords[i].x % 10;
					this->ships[i]->setPosition(position.x + (coords[i].x % 10) * cellSize.x, position.y + (coords[i].x / 10) * cellSize.y);
				}
			}
		}
	}

	void MapShape::setPosition(float x, float y)
	{
		this->setPosition(sf::Vector2f(x, y));
	}

	const sf::Vector2f& MapShape::getSize() const
	{
		return this->map.getSize();
	}

	const sf::Vector2f& MapShape::getPosition() const
	{
		return this->map.getPosition();
	}
}