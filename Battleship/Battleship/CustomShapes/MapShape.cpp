#include "pch.h"
#include "MapShape.h"

namespace Battleship
{
	MapShape::MapShape(BattleshipMap& battleshipMap, sf::Vector2f cellSize, bool empty, sf::Vector2f position, sf::Color cellFillColor, sf::Color cellOutlineColor)
		: cellSize(cellSize), battleshipMap(battleshipMap), empty(empty)
	{
		this->defaultCellColor = cellFillColor;
		this->cells = new sf::RectangleShape[CELLS_COUNT];
		this->initCells(cellOutlineColor);

		if (!this->empty)
		{
			this->ships = new ShipShape*[SHIPS_COUNT];
			this->shipsStartingPos = new sf::Vector2f[4];
			this->initShips();
		}

		this->map.setSize(cellSize * 10.f);
		this->map.setFillColor(sf::Color::Transparent);
		
		this->chosenCellColor = sf::Color(0, 0, 139);
		this->destroyedShipCellColor = sf::Color::Red;
		this->missedCellColor = sf::Color(0, 0, 100);

		this->initDraggingVariables();
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

	void MapShape::initCells(sf::Color cellOutlineColor)
	{
		sf::Vector2f mapPos = this->map.getPosition();

		for (int i = 0; i < CELLS_IN_ROW_COUNT; i++)
		{
			for (int j = 0; j < CELLS_IN_ROW_COUNT; j++)
			{
				this->cells[i * CELLS_IN_ROW_COUNT + j].setPosition(mapPos.x + j * cellSize.x, mapPos.y + i * cellSize.y);
				this->cells[i * CELLS_IN_ROW_COUNT + j].setSize(cellSize);

				this->cells[i * CELLS_IN_ROW_COUNT + j].setFillColor(this->defaultCellColor);
				this->cells[i * CELLS_IN_ROW_COUNT + j].setOutlineColor(cellOutlineColor);
				this->cells[i * CELLS_IN_ROW_COUNT + j].setOutlineThickness(-1.f);
			}
		}
	}

	void MapShape::initShips()
	{
		sf::Vector2f mapPos = this->map.getPosition();

		// set starting positions of ships
		this->initShipsStartingPos(mapPos);

		// if map is invalid clear it and set ships position to defalut
		// else read ships positions from battleshipMap
		if (!this->battleshipMap.validateMap())
		{
			this->battleshipMap.clear();
			
			for (int i = 1, k = 0; i < 5; i++)
			{
				for (int j = 0; j < 5 - i; j++, k++)
				{
					this->ships[k] = new ShipShape(i, this->cellSize, sf::Color(0, 255, 0, 128), sf::Color::White, -1.f);
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
				this->ships[i] = new ShipShape(iDiff + jDiff + 1, this->cellSize, sf::Color(0, 255, 0, 128), sf::Color::White, -1.f);
				if (jDiff > 0)
				{
					this->ships[i]->rotate();
				}
				this->ships[i]->setPosition(mapPos.x + (coords[i].x % 10) * cellSize.x, mapPos.y + (coords[i].x / 10) * cellSize.y);
				this->areShipsInDefaultPos = false;
			}
		}
	}

	void MapShape::initShipsStartingPos(const sf::Vector2f& mapPos)
	{
		for (int i = 0; i < 4; i++)
		{
			this->shipsStartingPos[i].x = mapPos.x + i * cellSize.x * 2;
			this->shipsStartingPos[i].y = mapPos.y - cellSize.x * 4;
		}
	}

	void MapShape::initDraggingVariables()
	{
		this->rotate = false;
		this->startDrag = false;
		this->returnToDefaultPos = false;
		this->dragShipIndex = 0;
		sf::Vector2f posDiff;
		sf::Vector2f startPosBackup;
		for (int i = 0; i < 10; i++)
		{
			dragShips[i] = false;
		}
	}

	void MapShape::destroyShipOnCell(int cellIndex)
	{
		this->cells[cellIndex].setFillColor(destroyedShipCellColor);
	}

	void MapShape::missOnCell(int cellIndex)
	{
		this->cells[cellIndex].setFillColor(missedCellColor);
	}

	void MapShape::chooseCell(int cellIndex)
	{
		this->cells[cellIndex].setFillColor(chosenCellColor);
	}

	void MapShape::rotateCurDraggedShip()
	{
		if (this->dragShips[dragShipIndex])
		{
			this->rotate = true;
		}
	}

	bool MapShape::saveMapShipsLocations()
	{
		BattleshipMap newMap;

		sf::FloatRect bounds;
		sf::Vector2i begin;
		sf::Vector2i end;
		sf::Vector2i mapPos(this->map.getPosition());

		for (int i = 0; i < 10; i++)
		{
			bounds = this->ships[i]->getGlobalBounds();
			begin.x = (int)(bounds.left - mapPos.x) / (int)cellSize.x;
			begin.y = (int)(bounds.top - mapPos.y) / (int)cellSize.y;
			end.x = begin.x + (int)(std::round(bounds.width) / cellSize.x) - 1;
			end.y = begin.y + (int)(std::round(bounds.height) / cellSize.y) - 1;
			for (int i = begin.y; i <= end.y; i++)
			{
				for (int j = begin.x; j <= end.x; j++)
				{
					newMap[i][j] = 1;
				}
			}
		}
		if (newMap.validateMap())
		{
			newMap.setFieldsLeft(20);
			this->battleshipMap.copy(newMap);
			return true;
		}
		return false;
	}

	void MapShape::setShipsPositionToDefault()
	{
		for (int i = 1, k = 0; i < 5; i++)
		{
			for (int j = 0; j < 5 - i; j++, k++)
			{
				this->ships[k]->setSize(sf::Vector2f(cellSize.x, cellSize.y * i));
				this->ships[k]->resetRotation();
				this->ships[k]->setPosition(shipsStartingPos[i - 1]);
			}
		}
		this->areShipsInDefaultPos = true;
	}

	void MapShape::updateShipsDragging(sf::Vector2f& mousePosView, bool& displayMessage)
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			for (int i = 9; i >= 0; i--)
			{
				if (dragShips[dragShipIndex] || ships[i]->getGlobalBounds().contains(mousePosView))
				{
					// saving starting position and ship index when drag started 
					if (!startDrag)
					{
						startPosBackup = ships[i]->getPosition();
						dragShipIndex = i;
						dragShips[dragShipIndex] = true;
						startDrag = true;
						posDiff = mousePosView - ships[i]->getPosition();
					}
					// some aligning when rotated
					if (rotate)
					{
						this->ships[dragShipIndex]->rotate();
						auto bounds = ships[dragShipIndex]->getGlobalBounds();
						startPosBackup = this->shipsStartingPos[__max((int)(std::round(bounds.width) / (int)cellSize.x), 
																(int)(std::round(bounds.height) / (int)cellSize.y)) - 1];
						this->returnToDefaultPos = true;
						
						ships[dragShipIndex]->setPosition(mousePosView - sf::Vector2f(bounds.width / 2, bounds.height / 2));
						
						posDiff = mousePosView - ships[dragShipIndex]->getPosition();
						rotate = false;
					}
					ships[dragShipIndex]->move(mousePosView - posDiff - ships[dragShipIndex]->getPosition());
					return;
				}
			}
		}
		// finished drag and checking if new position is valid
		if (startDrag)
		{
			sf::FloatRect bounds = ships[dragShipIndex]->getGlobalBounds();

			ships[dragShipIndex]->setPosition((bounds.left + cellSize.x / 2) - ((int)(bounds.left + cellSize.x / 2) % (int)cellSize.x),
				(bounds.top + cellSize.y / 2) - ((int)(bounds.top + cellSize.y / 2) % (int)cellSize.y));
			
			//checking if map contains center of both the ending and the begining of the ship
			if (!this->map.getGlobalBounds().contains(bounds.left + bounds.width - cellSize.x / 2, bounds.top + bounds.height - cellSize.y / 2) ||
				!this->map.getGlobalBounds().contains(bounds.left + cellSize.x / 2, bounds.top + cellSize.y / 2))
			{
				if (returnToDefaultPos)
				{
					ships[dragShipIndex]->resetRotation();
				}
				ships[dragShipIndex]->setPosition(startPosBackup);
			}
			this->areShipsInDefaultPos = false;
			this->returnToDefaultPos = false;
			dragShips[dragShipIndex] = false;
			startDrag = false;
			displayMessage = false;
		}
	}

	void MapShape::updateCellChoosingVisual(const sf::Vector2f& mousePos)
	{
		for (int i = 0; i < CELLS_COUNT; i++)
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && 
				this->cells[i].getGlobalBounds().contains(mousePos) && 
				this->cells[i].getFillColor() == this->defaultCellColor)
			{
				this->cells[i].setFillColor(this->chosenCellColor);
			}
			else if (cells[i].getFillColor() == chosenCellColor)
			{
				this->cells[i].setFillColor(defaultCellColor);
			}
		}
	}

	void MapShape::updateCellChoosing(const sf::Vector2f& mousePos, int& attackedCellIndex)
	{
		for (int i = 0; i < CELLS_COUNT; i++)
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->cells[i].getGlobalBounds().contains(mousePos))
			{
				attackedCellIndex = i;
			}
		}
	}

	void MapShape::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(map, states);
		for (int i = 0; i < CELLS_COUNT; i++)
		{
			target.draw(cells[i], states);
		}
		if (!this->empty)
		{
			for (int i = 0; i < SHIPS_COUNT; i++)
			{
				target.draw(*this->ships[i], states);
			}
		}
	}

	void MapShape::setPosition(const sf::Vector2f& position)
	{
		sf::Vector2f diff = position - this->map.getPosition();
		this->map.setPosition(position);
		for (int i = 0; i < CELLS_COUNT; i++)
		{
			this->cells[i].move(diff);
		}

		//reset ships starting positions

		if (!this->empty)
		{
			this->initShipsStartingPos(this->map.getPosition());
			for (int i = 0; i < SHIPS_COUNT; i++)
			{
				this->ships[i]->move(diff);
			}
		}
	}

	void MapShape::setPosition(float x, float y)
	{
		this->setPosition(sf::Vector2f(x, y));
	}

	void MapShape::setChosenCellColor(sf::Color color)
	{
		this->chosenCellColor = color;
	}

	void MapShape::setDestroyedShipCellColor(sf::Color color)
	{
		this->destroyedShipCellColor = color;
	}

	void MapShape::setMissedCellColor(sf::Color color)
	{
		this->missedCellColor = color;
	}

	void MapShape::setDefaultCellColor(sf::Color color)
	{
		this->defaultCellColor = color;
	}

	const sf::Vector2f& MapShape::getSize() const
	{
		return this->map.getSize();
	}

	const sf::Vector2f& MapShape::getPosition() const
	{
		return this->map.getPosition();
	}

	sf::FloatRect MapShape::getGlobalBounds() const
	{
		return this->map.getGlobalBounds();
	}

	bool MapShape::getAreShipsInDefaultPos() const
	{
		return this->areShipsInDefaultPos;
	}

	bool MapShape::getStartDrag() const
	{
		return this->startDrag;
	}

	sf::RectangleShape& MapShape::operator[](int i)
	{
		return this->cells[i];
	}
}