#include "pch.h"
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
		std::vector<std::string> menuOptionsText{ "Join", "Host game", "Edit map", "Match history", "Exit" };
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

	void MenuScreen::updateButtonsVisual()
	{
		for (auto& it : this->menuOptions)
		{
			it.second->update(data->inputManager.getMousePosView());
		}
		
	}

	void MenuScreen::updateButtonsFunction()
	{
		if (this->menuOptions["Join"]->isPressed())
		{
			if (!this->data->player.getMap().validateMap())
			{
				//this->data->screenManager.addScreen(new MapEditingScreen(data, new JoinScreen(this->data)), false);
				this->data->player.setMap("Utils/BattleshipMapInput2.txt");
				this->data->player.prepareDisplayalbeMaps();
				this->data->screenManager.addScreen(new JoinScreen(data), false);
			}
			else
			{
				this->data->player.prepareDisplayalbeMaps();
				this->data->screenManager.addScreen(new JoinScreen(data), false);
			}
		}
		if (this->menuOptions["Host game"]->isPressed())
		{
			if (!this->data->player.getMap().validateMap())
			{
				//this->data->screenManager.addScreen(new MapEditingScreen(data, new HostScreen(this->data)), false);
				this->data->player.setMap("Utils/BattleshipMapInput.txt");
				this->data->player.prepareDisplayalbeMaps();
				this->data->screenManager.addScreen(new HostScreen(data), false);
			}
			else
			{
				this->data->player.prepareDisplayalbeMaps();
				this->data->screenManager.addScreen(new HostScreen(data), false);
			}
		}
		if (this->menuOptions["Edit map"]->isPressed())
		{
			this->data->screenManager.addScreen(new MapEditingScreen(this->data, nullptr), false);
		}
		if (this->menuOptions["Match history"]->isPressed())
		{
			this->data->screenManager.addScreen(new MatchHistoryScreen(this->data), false);
		}
		if (this->menuOptions["Exit"]->isPressed())
		{
			this->data->window.close();
		}
	}

	void MenuScreen::update()
	{
		this->data->inputManager.updateMousePosView(this->data->window);
		this->updateButtonsVisual();
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
			if (sfEvent.type == sf::Event::MouseButtonPressed && sfEvent.mouseButton.button == sf::Mouse::Left)
			{
				this->updateButtonsFunction();
			}
		}
	}

	void MenuScreen::render()
	{
		this->data->window.clear();
		this->data->window.draw(this->background);
		for (auto& it : menuOptions)
		{
			this->data->window.draw(*it.second);
		}
		this->data->window.display();
	}
}