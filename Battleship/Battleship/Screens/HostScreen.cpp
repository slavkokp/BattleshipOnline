#include "pch.h"
#include "HostScreen.h"

namespace Battleship
{
	HostScreen::HostScreen(GameData* data) : data(data), serverPort(55555)
	{
		sf::Vector2f windowSize(this->data->window.getSize());
		this->exitButton = new Button(sf::Vector2f(windowSize.x / 5.f, windowSize.y / 3.f), "Back",
			this->data->resourceManager.getFont("ButtonFont"), 16, 22, sf::Color::Black);

		sf::Vector2f showcaseSize(130, 20);
		
		this->initIPshowcase(windowSize, showcaseSize);
		this->initShowcaseLabel(windowSize, showcaseSize);
		this->initWaitingMsg(windowSize, showcaseSize);
		
		bool firstTurn = rand() % 2;
		this->data->player.setFirstTurn(firstTurn);
		this->firstTurnPacket << !firstTurn;

		bool validPort = this->data->player.hostGame(serverPort);
		if (!validPort)
		{
			std::cerr << "invalid port\n";
		}
	}

	HostScreen::~HostScreen()
	{
		delete exitButton;
		delete IPshowcase;
	}

	void HostScreen::initIPshowcase(sf::Vector2f& windowSize, sf::Vector2f& showcaseSize)
	{
		this->IPshowcase = new InputField(showcaseSize);
		this->IPshowcase->setOutlineColor(sf::Color::White);
		this->IPshowcase->setString(sf::IpAddress::getLocalAddress().toString());
		this->IPshowcase->setFont(this->data->resourceManager.getFont("ButtonFont"));
		this->IPshowcase->setPosition(sf::Vector2f((windowSize.x - showcaseSize.x) / 2, (windowSize.y - showcaseSize.y) / 2));
	}

	void HostScreen::initShowcaseLabel(sf::Vector2f& windowSize, sf::Vector2f& showcaseSize)
	{
		this->showcaseLabel.setString("Your local IP (select and ctrl + c to copy):");
		this->showcaseLabel.setFont(this->data->resourceManager.getFont("ButtonFont"));
		this->showcaseLabel.setCharacterSize(16);
		this->showcaseLabel.setFillColor(sf::Color::White);
		sf::FloatRect bounds = this->showcaseLabel.getGlobalBounds();
		this->showcaseLabel.setPosition(sf::Vector2f((windowSize.x - bounds.width) / 2,
			(windowSize.y - showcaseSize.y) / 2 - bounds.height - 2.f));
		
	}

	void HostScreen::initWaitingMsg(sf::Vector2f& windowSize, sf::Vector2f& showcaseSize)
	{
		this->waitingMsg.setString("Waiting for connections...");
		this->waitingMsg.setFont(this->data->resourceManager.getFont("ButtonFont"));
		this->waitingMsg.setCharacterSize(16);
		this->waitingMsg.setFillColor(sf::Color::White);
		sf::FloatRect bounds = this->waitingMsg.getGlobalBounds();
		this->waitingMsg.setPosition(sf::Vector2f((windowSize.x - bounds.width) / 2,
			(windowSize.y + showcaseSize.y) / 2));
	}

	void HostScreen::updateButtonVisual()
	{
		this->exitButton->update(this->data->inputManager.getMousePosView());
	}

	void HostScreen::updateButtonFunction()
	{
		if (this->exitButton->isPressed())
		{
			this->data->screenManager.removeScreen();
		}
	}

	void HostScreen::update()
	{
		this->data->inputManager.updateMousePosView(this->data->window);
		this->updateButtonVisual();
		if (this->clock.getElapsedTime().asSeconds() > 0.3f)
		{
			this->updateButtonFunction();
		}
		// if connection established and first turn sent ==> go to game screen
		if (this->data->player.getConnection()->isReady(0, "") && this->data->player.sendFirstTurn(this->firstTurnPacket))
		{
			this->data->screenManager.addScreen(new GameScreen(data), true);
		}
	}

	void HostScreen::handleEvents()
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
				this->IPshowcase->update(this->data->inputManager.getMousePosView(), sfEvent.mouseButton.button);
			}
			if (sfEvent.type == sf::Event::KeyPressed)
			{
				this->IPshowcase->updateCopy(sfEvent);
			}
		}
	}

	void HostScreen::render()
	{
		this->data->window.clear();
		this->data->window.draw(*exitButton);
		this->data->window.draw(*this->IPshowcase);
		this->data->window.draw(this->showcaseLabel);
		this->data->window.draw(this->waitingMsg);
		this->data->window.display();
	}
}