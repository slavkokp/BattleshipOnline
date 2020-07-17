#include "pch.h"
#include "MapEditingScreen.h"

namespace Battleship
{
	MapEditingScreen::MapEditingScreen(GameData* data, bool callingByPlayButton) : data(data), cellSize(20, 20)
	{
		this->callingByPlayButton = callingByPlayButton;
		this->displayMessage = false;
		this->multipleSaveCallsProtection = false;
		this->returnToDefaultPos = false;
		this->dragStart = false;
		this->rotated = false;
		this->dragShips = new bool[10];
		this->dragShipIndex = 0;
		this->cells = new sf::RectangleShape[100];
		this->ships = new sf::RectangleShape[10];
		
		this->initBackgroundSprite();
		this->initButtons();
		this->initMap();
		this->initCells();
		this->initMessage();
		this->initDragShips();
		this->initShips();
		
		if (callingByPlayButton)
		{
			this->buttons["Save"]->setString("Continue");
		}
	}

	MapEditingScreen::~MapEditingScreen()
	{
		this->data->resourceManager.disposeTexture("MapEditingBackground");
		delete[] this->cells;
		delete[] this->ships;
		delete[] this->dragShips;
		for (auto& it : this->buttons)
		{
			delete it.second;
		}
	}

	void MapEditingScreen::initBackgroundSprite()
	{
		this->data->resourceManager.loadTextureFromFile("MapEditingBackground", "Resourses/MapEditingScreenBackground.jpg");
		sf::Texture& backgroundTexture = this->data->resourceManager.getTexture("MapEditingBackground");
		this->backgroundSprite.setTexture(backgroundTexture);
		if (backgroundTexture.getSize() != this->data->window.getSize())
		{
			ResourceManager::resizeSprite(backgroundSprite, (sf::Vector2f)this->data->window.getSize());
		}
	}

	void MapEditingScreen::initMessage()
	{
		this->message.setFont(this->data->resourceManager.getFont("ButtonFont"));
		this->message.setCharacterSize(16);
		sf::FloatRect bounds = this->map.getGlobalBounds();
		this->message.setPosition(bounds.left, bounds.top + bounds.height + cellSize.y);
		this->message.setFillColor(sf::Color::White);
	}

	void MapEditingScreen::initButtons()
	{
		std::string buttonsText[]{ "Back", "Reset", "Save" };
		for (int i = 0; i < 3; i++)
		{
			buttons[buttonsText[i]] = new Button(sf::Vector2f(this->data->window.getPosition().x / 7.f, this->data->window.getPosition().y / 7.f + 44.f * i), 
				buttonsText[i], this->data->resourceManager.getFont("ButtonFont"), 16, 22, sf::Color::Black, sf::Color::Magenta);
		}
	}

	void MapEditingScreen::initMap()
	{
		this->map.setSize(cellSize * 10.f);
		this->map.setPosition(this->data->window.getSize().x / 4.f, this->data->window.getSize().y / 4.f);
		this->map.setFillColor(sf::Color::Transparent);
	}

	void MapEditingScreen::initCells()
	{
		sf::Vector2f mapPos = this->map.getPosition();

		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				this->cells[i * 10 + j].setPosition(mapPos.x + i * cellSize.x, mapPos.y + j * cellSize.y);
				this->cells[i * 10 + j].setSize(cellSize);

				this->cells[i * 10 + j].setFillColor(sf::Color::Blue);
				this->cells[i * 10 + j].setOutlineColor(sf::Color::Black);
				this->cells[i * 10 + j].setOutlineThickness(-1.f);
			}
		}
	}

	void MapEditingScreen::initShips()
	{
		sf::Vector2f mapPos = this->map.getPosition();
		
		// set starting positions of ships
		for (int i = 0; i < 4; i++)
		{
			this->shipsStartingPos[i] = sf::Vector2f(mapPos.x + i * cellSize.x * 2, mapPos.y - cellSize.x * 4);
		}

		// if map is invalid clear it and set ships position to defalut;
		if (!this->data->player.getMap().validateMap())
		{
			this->data->player.getMap().clear();
			this->setShipsPositionToDefault();
			for (int i = 0; i < 10; i++)
			{
				this->ships[i].setFillColor(sf::Color::Green);
				this->ships[i].setOutlineColor(sf::Color::White);
				this->ships[i].setOutlineThickness(-1.f);
			}
		}
		else
		{
			auto coords = this->data->player.getMap().getShipsCoords();
			int iDiff;
			int jDiff;
			for (int i = 0; i < coords.size(); i++)
			{
				iDiff = coords[i].y / 10 - coords[i].x / 10;
				jDiff = coords[i].y % 10 - coords[i].x % 10;
				this->ships[i].setSize(sf::Vector2f(cellSize.x, cellSize.y * (iDiff + jDiff + 1)));
				if (jDiff > 0)
				{
					this->ships[i].rotate(270.f);
					this->ships[i].setPosition(mapPos.x + (coords[i].x % 10) * cellSize.x, mapPos.y + (coords[i].x / 10 + 1) * cellSize.y);
				}
				else
				{
					this->ships[i].setPosition(mapPos.x + (coords[i].x % 10) * cellSize.x, mapPos.y + (coords[i].x / 10) * cellSize.y);
				}
				this->ships[i].setFillColor(sf::Color::Green);
				this->ships[i].setOutlineColor(sf::Color::White);
				this->ships[i].setOutlineThickness(-1.f);
			}
		}
	}

	void MapEditingScreen::initDragShips()
	{
		for (int i = 0; i < 10; i++)
		{
			dragShips[i] = false;
		}
	}

	void MapEditingScreen::updateButtons()
	{
		for (auto& it : this->buttons)
		{
			it.second->update(this->data->inputManager.getMousePosView());
		}
		if (this->buttons["Back"]->isPressed())
		{
			this->data->screenManager.removeScreen();
		}
		if (this->buttons["Reset"]->isPressed())
		{
			if (!this->areShipsInDefaultPos)
			{
				this->setShipsPositionToDefault();
			}
		}
		if (this->buttons["Save"]->isPressed() && !multipleSaveCallsProtection)
		{
			if (this->saveMap())
			{
				this->data->player.prepareDisplayalbeMaps();
				this->message.setString("Map changes saved!");
				if (this->callingByPlayButton)
				{
					this->data->screenManager.addScreen(new GameScreen(this->data), true);
				}
			}
			else
			{
				this->message.setString("Invalid map, change location of ships and try again!");
			}
			this->displayMessage = true;
		}
	}

	void MapEditingScreen::setShipsPositionToDefault()
	{
		for (int i = 1, k = 0; i < 5; i++)
		{
			for (int j = 0; j < 5 - i; j++, k++)
			{
				this->ships[k].setSize(sf::Vector2f(cellSize.x, cellSize.y * i));
				this->ships[k].setPosition(shipsStartingPos[i - 1]);
				this->ships[k].setRotation(0.f);
			}
		}
		this->displayMessage = false;
		this->multipleSaveCallsProtection = false;
		this->areShipsInDefaultPos = true;
	}

	bool MapEditingScreen::saveMap()
	{
		BattleshipMap newMap;
		
		sf::FloatRect bounds;
		sf::Vector2i begin;
		sf::Vector2i end;
		sf::Vector2i mapPos(this->map.getPosition());
		bool rotated;
		
		for (int i = 0; i < 10; i++)
		{
			bounds = this->ships[i].getGlobalBounds();
			rotated = ships[i].getRotation() != 0.f;
			begin.x = (int)(bounds.left - mapPos.x) / (int)cellSize.x;
			begin.y = (int)(bounds.top - mapPos.y) / (int)cellSize.y;
			if (!rotated)
			{
				end.x = begin.x;
				end.y = begin.y + (int)(std::round(bounds.height) / cellSize.y) - 1;
			}
			else
			{
				end.y = begin.y;
				end.x = begin.x + (int)(std::round(bounds.width) / cellSize.x) - 1;
			}
			for (int i = begin.y; i <= end.y; i++)
			{
				for (int j = begin.x; j <= end.x; j++)
				{
					newMap[i][j] = 1;
				}
			}
		}
		multipleSaveCallsProtection = true;
		return this->data->player.setMap(newMap);
	}

	void MapEditingScreen::updateShipsDragging()
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			for (int i = 9; i >= 0; i--)
			{
				if (dragShips[dragShipIndex] || ships[i].getGlobalBounds().contains(this->data->inputManager.getMousePosView()))
				{
					// saving starting position and ship index when drag started 
					if (!dragStart)
					{

						this->displayMessage = false;
						startPosBackup = ships[i].getPosition();
						dragShipIndex = i;
						dragShips[dragShipIndex] = true;
						dragStart = true;
						posDiff = this->data->inputManager.getMousePosView() - ships[i].getPosition();
					}
					// some aligning when rotated
					if (rotated)
					{
						auto bounds = ships[dragShipIndex].getGlobalBounds();
						startPosBackup = this->shipsStartingPos[__max((int)(std::round(bounds.width) / (int)cellSize.x), 
																		(int)(std::round(bounds.height) / (int)cellSize.y)) - 1];
						this->returnToDefaultPos = true;
						ships[dragShipIndex].setPosition(this->data->inputManager.getMousePosView() - 
							sf::Vector2f(bounds.width / 2, (ships[dragShipIndex].getRotation() == 0.f ? bounds.height : -bounds.height) / 2));
						posDiff = this->data->inputManager.getMousePosView() - ships[dragShipIndex].getPosition();
						rotated = false;
					}
					ships[dragShipIndex].move(this->data->inputManager.getMousePosView() - posDiff  - ships[dragShipIndex].getPosition());
					return;
				}
			}
		}
		// finished drag and checking if new position is valid
		if (dragStart)
		{
			sf::FloatRect bounds = ships[dragShipIndex].getGlobalBounds();
			
			int mult = ships[dragShipIndex].getRotation() != 0 ? 3 : 1;
			ships[dragShipIndex].setPosition((bounds.left + cellSize.x / 2) - ((int)(bounds.left + cellSize.x / 2) % (int)cellSize.x),
				(bounds.top + cellSize.y / 2 * mult) - ((int)(bounds.top + cellSize.y / 2 * mult) % (int)cellSize.y));
			if (!this->map.getGlobalBounds().contains(bounds.left + bounds.width - cellSize.x / 2, bounds.top + bounds.height - cellSize.y / 2) ||
				!this->map.getGlobalBounds().contains(bounds.left + cellSize.x / 2, bounds.top + cellSize.y / 2))
			{
				ships[dragShipIndex].setPosition(startPosBackup);
				if (returnToDefaultPos)
				{
					ships[dragShipIndex].setRotation(0.f);
				}
			}
			this->areShipsInDefaultPos = false;
			this->returnToDefaultPos = false;
			dragShips[dragShipIndex] = false;
			dragStart = false;
			multipleSaveCallsProtection = false;
		}
	}

	void MapEditingScreen::update()
	{
		this->data->inputManager.updateMousePosView(this->data->window);
		updateShipsDragging();
		updateButtons();
	}

	void MapEditingScreen::handleEvents()
	{
		sf::Event sfEvent;
		while (this->data->window.pollEvent(sfEvent))
		{
			if (sfEvent.type == sf::Event::Closed)
			{
				this->data->window.close();
			}
			if (sfEvent.type == sf::Event::KeyPressed)
			{
				// ship rotate key
				if (sfEvent.key.code == sf::Keyboard::R)
				{
					if (this->dragShips[dragShipIndex])
					{
						if (this->ships[dragShipIndex].getRotation() == 0)
						{
							this->ships[dragShipIndex].rotate(270.f);
						}
						else
						{
							this->ships[dragShipIndex].rotate(90.f);
						}
						this->rotated = true;
					}
				}
			}
		}
	}

	void MapEditingScreen::render()
	{
		this->data->window.clear();
		this->data->window.draw(backgroundSprite);
		for (auto& it : this->buttons)
		{
			it.second->render(this->data->window);
		}
		this->data->window.draw(this->map);
		for (int i = 0; i < 100; i++)
		{
			this->data->window.draw(cells[i]);
		}
		for (int i = 0; i < 10; i++)
		{
			this->data->window.draw(ships[i]);
		}
		if (this->displayMessage)
		{
			this->data->window.draw(message);
		}
		this->data->window.display();
	}
}