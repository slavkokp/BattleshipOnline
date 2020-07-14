#include "Game.h"

namespace Battleship
{
	Game::Game(int windowWidth, int windowHeight) : gameData(new GameData()), user(gameData->player)
	{
		this->gameData->window.create(sf::VideoMode(windowWidth, windowHeight), "Battleship game", sf::Style::Default);
		this->gameData->screenManager.addScreen(new MenuScreen(this->gameData), false);
	}

	Game::~Game()
	{
		//temporary
		if (this->connection != nullptr)
		{
			delete this->connection;
		}
		delete this->gameData;
	}

	char Game::getSymbByValue(Row val)const
	{
		for (auto it : RowTable)
		{
			if (it.second == val)
			{
				return it.first;
			}
		}
		return '\0';
	}

	void Game::askPlayerName()
	{
		std::string name;
		std::cout << "Enter your nickname: ";
		std::cin >> name;
		this->user.setName(name);
	}

	bool Game::hostGame(unsigned short port)
	{
		Server* server = new Server();
		this->connection = server;
		return server->listen(port);
	}

	bool Game::joinGame(sf::IpAddress hostIP, unsigned short port)
	{
		Client* client = new Client();
		this->connection = client;
		return client->connect(hostIP, port);
	}

	void Game::startGame()
	{
		this->user.prepareDisplayalbeMaps();
		bool myTurn = this->firstTurn;
		std::string input;
		bool won = false;
		int row = 0;
		std::pair<Row, int> parsedInput;
		std::pair<bool, std::string> attackRes;
		bool isInputValid = false;
		while (true)
		{
			sf::Packet packet;
			if (myTurn)
			{
				while (!isInputValid)
				{
					this->user.printMaps();
					std::cout << "Enter position to be attacked: ";
					std::getline(std::cin, input);
					parsedInput = parseInput(input, isInputValid);
					if (!isInputValid)
					{
						std::cout << "Invalid input, try once more." << std::endl;
						system("pause");
						system("cls");
					}
				}
				isInputValid = false;
				packet << (int)(parsedInput.first) << parsedInput.second;
				this->connection->getSocket().send(packet);
				packet.clear();
				this->connection->getSocket().receive(packet);
				packet >> attackRes.first >> attackRes.second >> won;
				this->user.attack(parsedInput.first, parsedInput.second, attackRes.first);
				if (attackRes.first)
				{
					std::cout << "Successful attack! You " << attackRes.second << " enemy ship!" << std::endl;
				}
				else
				{
					std::cout << "You " << attackRes.second << std::endl;
				}
				if (won)
				{
					std::cout << this->user.getName() << " won!!!" << std::endl;
					system("pause");
					this->connection->getSocket().disconnect();
					break;
				}
				system("pause");
				system("CLS");
			}
			else
			{
				this->user.printMaps();
				std::cout << "Waiting for opponent's move... " << std::endl;
				this->connection->getSocket().receive(packet);
				packet >> row >> parsedInput.second;
				parsedInput.first = (Row)row;
				attackRes = this->user.takeDamage(parsedInput.first, parsedInput.second, won);
				packet.clear();
				packet << attackRes.first << attackRes.second << won;
				this->connection->getSocket().send(packet);
				system("CLS");
				this->user.printMaps();
				std::cout << "Opponent attacked " << getSymbByValue(parsedInput.first) << ' ' << parsedInput.second + 1 << std::endl;
				if (won)
				{
					std::cout << "You lost!!!" << std::endl;
					system("pause");
					this->connection->getSocket().disconnect();
					break;
				}
				system("pause");
				system("CLS");
			}
			myTurn = !myTurn;
		}
	}

	bool Game::sendFirstTurn()
	{
		if (this->connection != nullptr)
		{
			this->firstTurn = rand() % 2;
			sf::Packet packet;
			packet << !(this->firstTurn);
			this->connection->getSocket().send(packet);
			return true;
		}
		return false;
	}
	
	bool Game::receiveFirstTurn()
	{
		if (this->connection != nullptr)
		{
			sf::Packet packet;
			this->connection->getSocket().receive(packet);
			packet >> this->firstTurn;
			return true;
		}
		return false;
	}

	std::pair<Row, int> Game::parseInput(std::string input, bool& isInputValid)
	{
		std::pair<Row, int> res;
		auto it = RowTable.find(std::toupper(input[0]));
		if (it != RowTable.end())
		{
			res.first = it->second;
		}
		else
		{
			isInputValid = false;
			return res;
		}
		res.second = std::stoi(input.substr(1)) - 1;
		if (res.second < 0 || res.second > 9)
		{
			isInputValid = false;
			return res;
		}
		if (this->user.getOpponentDummy()[res.first][res.second] != 0)
		{
			isInputValid = false;
			return res;
		}
		isInputValid = true;
		return res;
	}

	bool Game::setMap(std::string filename)
	{
		return this->user.setMap(filename);
	}

	// GUI methods

	void Game::run()
	{
		while (this->gameData->window.isOpen())
		{
			this->gameData->screenManager.handleScreenSwitches();
			this->gameData->screenManager.getCurrentScreen().handleEvents();
			this->gameData->screenManager.getCurrentScreen().update();
			this->gameData->screenManager.getCurrentScreen().render();
		}
	}
}