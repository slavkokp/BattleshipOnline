#include "pch.h"
#include "GameScreen.h"

namespace Battleship
{
	GameScreen::GameScreen(GameData* data) : data(data), playerClone(data->player.getMapClone(), sf::Vector2f(20, 20), false),
		opponentsDummy(data->player.getOpponentDummy(), sf::Vector2f(20, 20), true)
	{
		opponentsDummy.setPosition(sf::Vector2f(this->data->window.getSize().x / 2.f - 300, this->data->window.getSize().y / 2.f - 100));
		playerClone.setPosition(sf::Vector2f(this->data->window.getSize().x / 2.f + 100, this->data->window.getSize().y / 2.f - 100));
	}

	GameScreen::~GameScreen()
	{
	}

	void GameScreen::update()
	{
		this->data->inputManager.updateMousePosView(this->data->window);
		this->opponentsDummy.updateCellChoosing(this->data->inputManager.getMousePosView());
	}

	void GameScreen::handleEvents()
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
				if (sfEvent.key.code == sf::Keyboard::Escape)
				{
					this->data->screenManager.removeScreen();
				}
			}
		}
	}

	void GameScreen::render()
	{
		this->data->window.clear();
		this->data->window.draw(playerClone);
		this->data->window.draw(opponentsDummy);
		this->data->window.display();
	}
}