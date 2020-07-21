#include "pch.h"
#include "Player.h"

namespace Battleship
{
	Player::Player(std::string name): name(name), connection(nullptr) {}

	Player::~Player()
	{
		if (this->connection != nullptr)
		{
			delete connection;
		}
	}

	std::string Player::getName()const
	{
		return name;
	}

	void Player::attack(Row row, int col, bool success)
	{
		this->opponentDummy[(int)row][col] = success ? Hit : Missed;
	}

	std::pair<bool, std::string> Player::takeDamage(Row row, int col, bool& wonGame)
	{
		auto res = this->mapClone.attack((int)row, col);
		wonGame = this->mapClone.getFieldsLeft() == 0 ? true : false;
		return res;
	}

	bool Player::hostGame(unsigned short port)
	{
		if (this->connection != nullptr)
		{
			delete connection;
		}
		Server* server = new Server();
		this->connection = server;
		return server->listen(port);
	}

	void Player::joinGame()
	{
		if (this->connection != nullptr)
		{
			delete connection;
		}
		Client* client = new Client();
		this->connection = client;
	}

	bool Player::sendFirstTurn(sf::Packet& packet)
	{
		if (this->connection->getSocket().getRemotePort() != 0)
		{
			if (this->connection->getSocket().send(packet) != sf::Socket::Done)
			{
				return false;
			}
			return true;
		}
		return false;
	}

	bool Player::receiveFirstTurn(sf::Packet& packet)
	{
		if (this->connection->getSocket().getRemotePort() != 0)
		{
			if (this->connection->getSocket().receive(packet) != sf::Socket::Done)
			{
				return false;
			}
			packet >> this->firstTurn;
			return true;
		}
		return false;
	}

	bool Player::setMap(std::string filename)
	{
		bool isValid = this->map.readMapFromFile(filename);
		if (!isValid)
		{
			return isValid;
		}
		this->mapClone.copy(this->map);
		return isValid;
	}

	bool Player::setMap(BattleshipMap map)
	{
		if (map.validateMap())
		{
			this->map.copy(map);
			return true;
		}
		return false;
	}

	void Player::setName(std::string name)
	{
		this->name = name;
	}

	void Player::setFirstTurn(bool firstTurn)
	{
		this->firstTurn = firstTurn;
	}

	void Player::printMaps()const
	{
		std::cout << "Opponent's map\t\tYour map" << std::endl;
		BattleshipMap::printMapsInRow(this->opponentDummy, this->mapClone);
	}

	void Player::prepareDisplayalbeMaps()
	{
		this->mapClone.copy(this->map);
		this->opponentDummy.clear();
	}

	BattleshipMap& Player::getMapClone()
	{
		return this->mapClone;
	}

	BattleshipMap& Player::getMap()
	{
		return this->map;
	}

	BattleshipMap& Player::getOpponentDummy()
	{
		return this->opponentDummy;
	}

	Connection* Player::getConnection()
	{
		return this->connection;
	}

	bool Player::getFirstTurn()const
	{
		return this->firstTurn;
	}

	char Player::getMapSymbByValue(Row val)
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
}