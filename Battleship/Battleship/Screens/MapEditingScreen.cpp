#include "pch.h"
#include "MapEditingScreen.h"

namespace Battleship
{
	MapEditingScreen::MapEditingScreen(GameData* data, Screen* nextScreenPtr) : data(data), cellSize(20, 20)
	{
		this->map = new MapShape(this->data->player.getMap(), cellSize, false);
		this->map->setPosition(this->data->window.getSize().x / 4.f, this->data->window.getSize().y / 4.f);

		this->nextScreenPtr = nextScreenPtr;
		this->displayMessage = false;
		
		this->initBackgroundSprite();
		this->initButtons();
		this->initMessage();
		
		if (this->nextScreenPtr != nullptr)
		{
			this->buttons["Save"]->setString("Continue");
		}
	}

	MapEditingScreen::~MapEditingScreen()
	{
		this->data->resourceManager.disposeTexture("MapEditingBackground");
		delete map;
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
		sf::FloatRect bounds = this->map->getGlobalBounds();
		this->message.setPosition(bounds.left, bounds.top + bounds.height + cellSize.y);
		this->message.setFillColor(sf::Color::White);
	}

	void MapEditingScreen::initButtons()
	{
		std::string buttonsText[]{ "Back", "Reset", "Save" };
		for (int i = 0; i < 3; i++)
		{
			buttons[buttonsText[i]] = new Button(sf::Vector2f(this->data->window.getSize().x / 7.f, this->data->window.getSize().y / 7.f + 44.f * i), 
				buttonsText[i], this->data->resourceManager.getFont("ButtonFont"), 16, 22, sf::Color::Black, sf::Color::Magenta);
		}
	}

	void MapEditingScreen::updateButtonsVisual()
	{
		for (auto& it : this->buttons)
		{
			it.second->update(this->data->inputManager.getMousePosView());
		}
	}

	void MapEditingScreen::updateButtonsFunction()
	{
		if (this->buttons["Back"]->isPressed())
		{
			if (this->nextScreenPtr != nullptr)
			{
				delete nextScreenPtr;
			}
			this->data->screenManager.removeScreen();
		}
		if (this->buttons["Reset"]->isPressed())
		{
			if (!this->map->getAreShipsInDefaultPos())
			{
				this->setShipsPositionToDefault();
			}
		}
		if (this->buttons["Save"]->isPressed() && !displayMessage)
		{
			if (this->saveMap())
			{
				this->data->player.prepareDisplayalbeMaps();
				this->message.setString("Map changes saved!");
				if (this->nextScreenPtr != nullptr)
				{
					this->data->screenManager.addScreen(this->nextScreenPtr, true);
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
		this->map->setShipsPositionToDefault();
		this->displayMessage = false;
	}

	bool MapEditingScreen::saveMap()
	{
		return this->map->saveMapShipsLocations();
	}

	void MapEditingScreen::updateShipsDragging()
	{
		this->map->updateShipsDragging(this->data->inputManager.getMousePosView(), displayMessage);
	}

	void MapEditingScreen::update()
	{
		this->data->inputManager.updateMousePosView(this->data->window);
		this->updateShipsDragging();
		this->updateButtonsVisual();
	}

	void MapEditingScreen::handleEvents()
	{
		sf::Event sfEvent;
		while (this->data->window.pollEvent(sfEvent))
		{
			if (sfEvent.type == sf::Event::Closed)
			{
				if (this->nextScreenPtr != nullptr)
				{
					delete nextScreenPtr;
				}
				this->data->window.close();
			}
			if (sfEvent.type == sf::Event::KeyPressed)
			{
				// ship rotate key
				if (sfEvent.key.code == sf::Keyboard::R)
				{
					this->map->rotateCurDraggedShip();
				}
			}
			if (sfEvent.type == sf::Event::MouseButtonPressed && sfEvent.mouseButton.button == sf::Mouse::Left)
			{
				this->updateButtonsFunction();
			}
		}
	}

	void MapEditingScreen::render()
	{
		this->data->window.clear();
		this->data->window.draw(backgroundSprite);
		for (auto& it : this->buttons)
		{
			this->data->window.draw(*it.second);
		}
		this->data->window.draw(*this->map);
		if (this->displayMessage)
		{
			this->data->window.draw(message);
		}
		this->data->window.display();
	}
}