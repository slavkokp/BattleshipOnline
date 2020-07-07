#pragma once
#include <cctype>
#include "Player.h"
#include "Connection.h"
#include <sstream>

namespace Battleship
{
	class Game
	{
	private:
		Player user;
		Connection* connection = nullptr; // server or client
		bool isConnected;
		char getSymbByValue(Row val)const;
		bool firstTurn;
		std::pair<Row, int> parseInput(std::string input, bool& isInputValid);
	public:
		Game();
		void askPlayerName();
		void startGame();
		bool setMap(std::string filename);
		
		// if connection established, randomly sets firstTurn and sends it to the opponent
		bool sendFirstTurn();

		// if connection established, receives firstTurn packet
		bool receiveFirstTurn();

		bool hostGame(unsigned short port);
		bool joinGame(sf::IpAddress hostIP, unsigned short port);
		~Game();
	};
}