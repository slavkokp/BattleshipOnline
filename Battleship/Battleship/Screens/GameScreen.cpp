#include "pch.h"
#include "GameScreen.h"

namespace Battleship
{
	GameScreen::GameScreen(GameData* data) : data(data), playerClone(data->player.getMapClone(), sf::Vector2f(20, 20), false),
		opponentsDummy(data->player.getOpponentDummy(), sf::Vector2f(20, 20), true), opponentSocket(data->player.getConnection()->getSocket())
	{
		sf::Vector2f windowSize(this->data->window.getSize());
		this->opponentsDummy.setPosition(sf::Vector2f(windowSize.x / 2.f - 300, windowSize.y / 2.f - 100));
		this->opponentsDummy.setDestroyedShipCellColor(sf::Color::Green);

		this->playerClone.setPosition(sf::Vector2f(windowSize.x / 2.f + 100, windowSize.y / 2.f - 100));
		
		this->myTurn = data->player.getFirstTurn();
		
		this->initGameStatusMsg(windowSize);
		for (int i = 0; i < 100; i++)
		{
			this->cellStatusArr[i] = false;
		}
		this->cellStatusArr[100] = true;
		this->attackedCellIndex = 100;

		this->initButtons(windowSize);
		this->drawOKbtn = false;

		this->startSendingInput = false;
		this->startSendingAttackRes = false;
		this->startRecievingAttackRes = false;
		this->startRecievingInput = false;
		this->won = false;
		this->waitingForPlayer = true;
		this->missed = false;
	}

	GameScreen::~GameScreen()
	{
		for (auto& it : this->buttons)
		{
			delete it.second;
		}
		this->data->player.getConnection()->dispose();
	}

	void GameScreen::initButtons(sf::Vector2f& windowSize)
	{
		std::string text[] { "MainMenu", "OK" };
		sf::FloatRect bounds = this->gameStatusMsg.getGlobalBounds();
		sf::Vector2f pos[] { sf::Vector2f(windowSize.x / 10.f, windowSize.y / 6.f), 
			sf::Vector2f((windowSize.x - 20.f) / 2.f, bounds.top + bounds.height) };
		for (int i = 0; i < 2; i++)
		{
			this->buttons[text[i]] = new Button(pos[i], text[i], this->data->resourceManager.getFont("ButtonFont"), 16, 22, sf::Color::Black);
		}
	}

	void GameScreen::initGameStatusMsg(sf::Vector2f& windowSize)
	{
		if (this->myTurn)
		{
			this->gameStatusMsg.setString("Pick a field to be attacked");
		}
		else
		{
			this->gameStatusMsg.setString("Wait for opponent to attack");
		}
		this->gameStatusMsg.setCharacterSize(16);
		this->gameStatusMsg.setFillColor(sf::Color::White);
		this->gameStatusMsg.setFont(this->data->resourceManager.getFont("ButtonFont"));
		sf::FloatRect bounds = this->gameStatusMsg.getGlobalBounds();
		this->gameStatusMsg.setPosition(sf::Vector2f((windowSize.x - bounds.width) / 2,
			(windowSize.y + opponentsDummy.getSize().y) / 2));
	}

	void GameScreen::setGameStatusMsgString(std::string msg)
	{
		sf::FloatRect bounds = gameStatusMsg.getGlobalBounds();
		this->gameStatusMsg.setString(msg);
		this->gameStatusMsg.move(sf::Vector2f((this->data->window.getSize().x - gameStatusMsg.getGlobalBounds().width) / 2 - bounds.left, 0));
	}

	void GameScreen::sendInput()
	{
		sf::Socket::Status status = this->opponentSocket.send(this->packet);
		if (status == sf::Socket::Done)
		{
			this->startSendingInput = false;
			this->startRecievingAttackRes = true;
			this->packet.clear();
		}
		else
		{
			this->updateConnectionStatus(status);
		}
	}

	void GameScreen::sendAttackRes()
	{
		sf::Socket::Status status = this->opponentSocket.send(this->packet);
		if (status == sf::Socket::Done)
		{
			this->packet.clear();
			this->startSendingAttackRes = false;
			this->waitingForPlayer = true;
			this->myTurn = !myTurn;
		}
		else
		{
			this->updateConnectionStatus(status);
		}
	}

	void GameScreen::receiveInput()
	{
		sf::Socket::Status status = this->opponentSocket.receive(this->packet);
		if (status == sf::Socket::Done)
		{
			int row;
			int col;
			packet >> row >> col;
			auto res = this->data->player.takeDamage((Row)row, col, won);
			if (won) 
			{
				this->setGameStatusMsgString("You lost!");
			}
			else
			{
				this->setGameStatusMsgString("Opponent attacked " + std::string(1, this->data->player.getMapSymbByValue((Row)row)) + std::to_string(col + 1));
			}
			packet.clear();
			
			this->missed = !res.first;
			this->msg = res.second;
			// filling packet to send opponent's attack res back
			this->packet << missed << msg << won;
			this->startSendingAttackRes = true;
			this->startRecievingInput = false;
			this->drawOKbtn = true;
			if (this->missed)
			{
				this->playerClone.missOnCell(row * 10 + col);
			}
			else
			{
				this->playerClone.destroyShipOnCell(row * 10 + col);
			}
		}
		else
		{
			this->updateConnectionStatus(status);
		}
	}

	void GameScreen::receiveAttackRes()
	{
		sf::Socket::Status status = this->opponentSocket.receive(this->packet);
		if (status == sf::Socket::Done)
		{
			this->startRecievingAttackRes = false;
			this->waitingForPlayer = true;
			this->myTurn = !myTurn;
			this->packet >> this->missed >> msg >> this->won;
			if (this->missed)
			{
				this->opponentsDummy.missOnCell(attackedCellIndex);
				this->setGameStatusMsgString("You " + msg);
			}
			else
			{
				this->opponentsDummy.destroyShipOnCell(attackedCellIndex);
				this->setGameStatusMsgString("Successful attack! You " + msg + " enemy ship!");
			}
			if (won)
			{
				this->setGameStatusMsgString("You won!");
			}
			this->drawOKbtn = true;
			this->packet.clear();
		}
		else
		{
			this->updateConnectionStatus(status);
		}
	}

	void GameScreen::attack(int cellIndex)
	{
		this->attackedCellIndex = cellIndex;
		this->packet << cellIndex / 10 << cellIndex % 10;
		this->startSendingInput = true;
	}

	void GameScreen::updateConnectionStatus(sf::Socket::Status& status)
	{
		if (status == sf::Socket::Disconnected)
		{
			this->setGameStatusMsgString("You won, opponent disconnected!");
			this->won = true;
			this->drawOKbtn = true;
		}
	}

	void GameScreen::updateButtonsVisual()
	{
		for (auto& it : this->buttons)
		{
			it.second->update(this->data->inputManager.getMousePosView());
		}
	}

	void GameScreen::updateButtonsFunction()
	{
		if (this->buttons["MainMenu"]->isPressed())
		{
			this->data->screenManager.removeScreen();
		}
		if (this->buttons["OK"]->isPressed())
		{
			if (this->won)
			{
				this->data->screenManager.removeScreen();
			}
			if (this->myTurn)
			{
				this->setGameStatusMsgString("Pick a field to be attacked");
				this->drawOKbtn = false;
			}
			else
			{
				this->setGameStatusMsgString("Wait for opponent to attack");
				this->drawOKbtn = false;
			}
		}
	}

	void GameScreen::update()
	{
		this->data->inputManager.updateMousePosView(this->data->window);
		this->updateButtonsVisual();
		if (this->clock.getElapsedTime().asSeconds() > 0.3f)
		{
			this->updateButtonsFunction();
			this->opponentsDummy.updateCellChoosingVisual(this->data->inputManager.getMousePosView());
			if (this->waitingForPlayer)
			{
				if (this->myTurn)
				{
					this->opponentsDummy.updateCellChoosing(this->data->inputManager.getMousePosView(), this->attackedCellIndex);
					if (!this->cellStatusArr[attackedCellIndex])
					{
						this->attack(this->attackedCellIndex);
						this->cellStatusArr[attackedCellIndex] = true;
						this->waitingForPlayer = false;
					}
					else
					{
						return;
					}
				}
				else
				{
					this->startRecievingInput = true;
					this->waitingForPlayer = false;
				}
			}
		}
		if (this->startSendingInput)
		{
			this->sendInput();
		}
		if (this->startRecievingAttackRes)
		{
			this->receiveAttackRes();
		}
		if (this->startRecievingInput)
		{
			this->receiveInput();
		}
		if (this->startSendingAttackRes)
		{
			this->sendAttackRes();
		}
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
		this->data->window.draw(*buttons["MainMenu"]);
		if (this->drawOKbtn)
		{
			this->data->window.draw(*buttons["OK"]);
		}
		this->data->window.draw(playerClone);
		this->data->window.draw(opponentsDummy);
		this->data->window.draw(gameStatusMsg);
		this->data->window.display();
	}
}