#include "pch.h"
#include "JoinScreen.h"

namespace Battleship
{
	JoinScreen::JoinScreen(GameData* data) : data(data)
	{
		sf::Vector2f size(130, 20);
		sf::Vector2f windowSize(this->data->window.getSize());
		this->inputField = new InputField(size, sf::Vector2f((windowSize.x - size.x) / 2 , (windowSize.y - size.y) / 2),
			this->data->resourceManager.getFont("ButtonFont"), "255.255.255.255", sf::Color::Green);
		
		this->connectionFailed = false;

		this->initButtons(windowSize);
		this->initMessages(windowSize, size);
	}

	JoinScreen::~JoinScreen()
	{
		delete this->inputField;
		for (auto& it : this->buttons)
		{
			delete it.second;
		}
	}

	void JoinScreen::initButtons(sf::Vector2f& windowSize)
	{
		std::string options[]{ "Back", "Connect" };

		for (int i = 0; i < 2; i++)
		{
			this->buttons[options[i]] = new Button(sf::Vector2f(windowSize.x / 5.f, windowSize.y / 3.f + 44.f * i),
				options[i], data->resourceManager.getFont("ButtonFont"), 16, 22, sf::Color::Black);
		}
	}

	void JoinScreen::initMessages(sf::Vector2f& windowSize, sf::Vector2f& size)
	{
		this->inputMsg.setString("Insert host IP:");
		this->inputMsg.setFont(this->data->resourceManager.getFont("ButtonFont"));
		this->inputMsg.setCharacterSize(16);
		this->inputMsg.setFillColor(sf::Color::White);
		this->inputMsg.setPosition(sf::Vector2f((windowSize.x - size.x) / 2, 
			(windowSize.y - size.y) / 2 - inputMsg.getGlobalBounds().height - 2.f));

		this->connectionFailureMsg.setString("Cannot connect to host!");
		this->connectionFailureMsg.setFont(this->data->resourceManager.getFont("ButtonFont"));
		this->connectionFailureMsg.setCharacterSize(16);
		this->connectionFailureMsg.setFillColor(sf::Color::White);
		this->connectionFailureMsg.setPosition(sf::Vector2f((windowSize.x - size.x) / 2,
			(windowSize.y + size.y) / 2));
	}

	void JoinScreen::updateButtonsVisual()
	{
		for (auto& it : this->buttons)
		{
			it.second->update(this->data->inputManager.getMousePosView());
		}
	}

	void JoinScreen::updateButtonsFunction()
	{
		if (buttons["Back"]->isPressed())
		{
			this->data->screenManager.removeScreen();
		}
		if (buttons["Connect"]->isPressed())
		{
		}
	}

	void JoinScreen::update()
	{
		this->data->inputManager.updateMousePosView(this->data->window);
		this->updateButtonsVisual();
		if (clock.getElapsedTime().asSeconds() > 0.3f)
		{
			this->updateButtonsFunction();
		}
	}

	void JoinScreen::handleEvents()
	{
		sf::Event sfEvent;
		while (this->data->window.pollEvent(sfEvent))
		{
			if (sfEvent.type == sf::Event::Closed)
			{
				this->data->window.close();
			}
			if (sfEvent.type == sf::Event::MouseButtonPressed)
			{
				this->inputField->update(this->data->inputManager.getMousePosView(), sfEvent.mouseButton.button);
			}
			if (sfEvent.type == sf::Event::KeyPressed)
			{
				this->inputField->updateCopy(sfEvent);
				this->inputField->updatePaste(sfEvent);
				continue;
			}
			if (sfEvent.type == sf::Event::TextEntered)
			{
				// backspace || dot || digits
				if (sfEvent.text.unicode == 8 || sfEvent.text.unicode == 46 || (sfEvent.text.unicode >= 48 && sfEvent.text.unicode <= 57))
				{
					this->inputField->updateText(sfEvent.text.unicode);
				}
			}
		}
	}
	
	void JoinScreen::render()
	{
		this->data->window.clear();
		this->data->window.draw(*inputField);
		for (auto& it : this->buttons)
		{
			this->data->window.draw(*it.second);
		}
		this->data->window.draw(this->inputMsg);
		if (this->connectionFailed)
		{
			this->data->window.draw(this->connectionFailureMsg);
		}
		this->data->window.display();
	}
}