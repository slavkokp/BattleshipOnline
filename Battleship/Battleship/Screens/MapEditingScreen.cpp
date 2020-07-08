#include "MapEditingScreen.h"

namespace Battleship
{
	MapEditingScreen::MapEditingScreen(GameData* data) : data(data)
	{
		this->test.setFillColor(sf::Color::Red);
		this->test.setSize(sf::Vector2f(200.f, 200.f));
	}

	MapEditingScreen::~MapEditingScreen()
	{

	}

	void MapEditingScreen::update()
	{

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
		}
	}

	void MapEditingScreen::render()
	{
		this->data->window.clear();
		this->data->window.draw(this->test);
		this->data->window.display();
	}
}