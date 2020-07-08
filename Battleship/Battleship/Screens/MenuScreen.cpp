#include "MenuScreen.h"

namespace Battleship
{
	MenuScreen::MenuScreen(GameData* data) : data(data)
	{
		testShape.setRadius(100.f);
		testShape.setFillColor(sf::Color::Cyan);
	}

	MenuScreen::~MenuScreen()
	{
		
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
			if (sfEvent.type == sf::Event::KeyPressed)
			{
				if (sfEvent.key.code == sf::Keyboard::Enter)
				{
					this->data->screenManager.addScreen(new MapEditingScreen(this->data), true);
				}
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