#include "pch.h"
#include "JoinScreen.h"

namespace Battleship
{
	JoinScreen::JoinScreen(GameData* data) : data(data)
	{
		sf::Vector2f size(130, 20);
		sf::Vector2f windowSize(this->data->window.getSize());

		this->connectionFailed = false;
		this->connecting = false;
		this->sending = false;
		this->receiving = false;

		this->initInputField(windowSize, size);
		this->initButtons(windowSize);
		this->initMessages(windowSize, size);
		this->data->player.joinGame();
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

		this->connectionMsg.setString("Cannot connect to host!");
		this->connectionMsg.setFont(this->data->resourceManager.getFont("ButtonFont"));
		this->connectionMsg.setCharacterSize(16);
		this->connectionMsg.setFillColor(sf::Color::White);
		this->connectionMsg.setPosition(sf::Vector2f((windowSize.x - size.x) / 2,
			(windowSize.y + size.y) / 2));
	}

	void JoinScreen::initInputField(sf::Vector2f& windowSize, sf::Vector2f& size)
	{
		this->inputField = new InputField(size);
		this->inputField->setFont(this->data->resourceManager.getFont("ButtonFont"));
		this->inputField->setOutlineColor(sf::Color::Green);
		this->inputField->setPosition(sf::Vector2f((windowSize.x - size.x) / 2, (windowSize.y - size.y) / 2));
	}


	bool JoinScreen::sendName()
	{
		if (this->data->player.getConnection()->getSocket().getRemotePort() != 0)
		{
			this->sending = true;
			if (this->data->player.getConnection()->getSocket().send(this->namePacket) != sf::Socket::Done)
			{
				return false;
			}
			this->sending = false;
			this->namePacket.clear();
			return true;
		}
		return false;
	}

	bool JoinScreen::receiveName()
	{
		if (this->data->player.getConnection()->getSocket().getRemotePort() != 0)
		{
			this->receiving = true;
			if (this->data->player.getConnection()->getSocket().receive(this->namePacket) != sf::Socket::Done)
			{
				return false;
			}
			this->receiving = false;
			this->namePacket >> this->nameOfOpponent;
			this->namePacket.clear();
			this->namePacket << this->data->player.getName();
			return true;
		}
		return false;
	}


	void JoinScreen::updateConnecting()
	{
		if (sending || receiving || (this->data->player.getConnection()->isReady(55555, this->inputField->getText()) &&
			this->data->player.receiveFirstTurn(this->firstTurnPacket)))
		{
			if (sending || this->receiveName())
			{
				if (this->sendName())
				{
					this->data->screenManager.addScreen(new GameScreen(this->data, nameOfOpponent), true);
				}
			}
		}
		if (this->connectionTimer.getElapsedTime().asSeconds() > 10.f)
		{
			this->connecting = false;
			this->connectionFailed = true;
			this->connectionMsg.setString("Cannot connect to host!");
		}
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
			this->connecting = true;
			this->connectionFailed = false;
			this->connectionMsg.setString("Connecting...");
			connectionTimer.restart();
		}
	}

	void JoinScreen::update()
	{
		this->data->inputManager.updateMousePosView(this->data->window);
		this->updateButtonsVisual();
		if (this->connecting)
		{
			this->updateConnecting();
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
				if (sfEvent.mouseButton.button == sf::Mouse::Left)
				{
					this->updateButtonsFunction();
				}
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
		if (this->connectionFailed || this->connecting)
		{
			this->data->window.draw(this->connectionMsg);
		}
		this->data->window.display();
	}
}