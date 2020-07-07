#include "MenuScreen.h"

namespace Battleship
{
	MenuScreen::MenuScreen(GameData* data) : data(data)
	{
		testShape.setRadius(100.f);
		testShape.setFillColor(sf::Color::Cyan);
	}

	void MenuScreen::update()
	{
	}

	void MenuScreen::handleEvents()
	{
		sf::Event sfEvent;
		while (this->data->window.pollEvent(sfEvent))
		{
			if (sfEvent.type == sf::Event::Closed)
			{
				this->data->window.close();
			}
			if (sfEvent.type == sf::Event::Resized)
			{
				printf("Height: %d, Width: %d\n", sfEvent.size.height, sfEvent.size.width);
			}
		}
	}

	void MenuScreen::render()
	{
		this->data->window.clear();
		this->data->window.draw(this->testShape);
		this->data->window.display();
	}

	
}