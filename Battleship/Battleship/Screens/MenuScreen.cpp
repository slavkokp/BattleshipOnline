#include "MenuScreen.h"

namespace Battleship
{
	MenuScreen::MenuScreen(GameData* data) : data(data)
	{
		testShape.setRadius(100.f);
		testShape.setFillColor(sf::Color::Cyan);
		this->data->resourceManager.loadTextureFromFile("MainMenuBackground", "Resourses/MainMenuBackground.jpg");
		sf::Texture& backgroundTexture = this->data->resourceManager.getTexture("MainMenuBackground");
		this->background.setTexture(backgroundTexture);
		if (backgroundTexture.getSize() != this->data->window.getSize())
		{
			ResourceManager::resizeSprite(background, (sf::Vector2f)this->data->window.getSize());
		}
		
		data->resourceManager.loadFontFromFile("ButtonFont", "Resourses/VINERITC.TTF");
		std::vector<std::string> menuOptionsText{ "Play", "Edit map", "Exit" };
		for (int i = 0; i < menuOptionsText.size(); i++)
		{
			this->menuOptions[menuOptionsText[i]] = new Button(sf::Vector2f(this->data->window.getSize().x / 5.f, this->data->window.getSize().y / 3.f + 44.f * i), 
															menuOptionsText[i], data->resourceManager.getFont("ButtonFont"), 16, 22, sf::Color::Black);
		}
	}

	MenuScreen::~MenuScreen()
	{
		this->data->resourceManager.disposeTexture("MainMenuBackground");
		for (auto& it : this->menuOptions)
		{
			delete it.second;
		}
	}

	void MenuScreen::updateMenuOptions()
	{
		for (auto& it : this->menuOptions)
		{
			it.second->update(data->inputManager.getMousePosView());
		}
		if (this->menuOptions["Exit"]->isPressed())
		{
			this->data->window.close();
		}
		if (this->menuOptions["Edit map"]->isPressed())
		{
			this->data->screenManager.addScreen(new MapEditingScreen(this->data), false);
		}
	}

	void MenuScreen::update()
	{
		this->data->inputManager.updateMousePosView(this->data->window);
		this->updateMenuOptions();
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
		this->data->window.draw(this->background);
		for (auto& it : menuOptions)
		{
			it.second->render(this->data->window);
		}
		this->data->window.display();
	}
}