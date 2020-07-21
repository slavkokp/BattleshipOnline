#pragma once

#include <cctype>
#include <sstream>
#include "../Network/Connection.h"
#include "../Screens/MenuScreen.h"

namespace Battleship
{
	class Game
	{
	private:
		Player& user;
		Connection* connection; // server or client
		bool firstTurn;
		
		//private methods

		char getSymbByValue(Row val)const;
		std::pair<Row, int> parseInput(std::string input, bool& isInputValid);

		//GUI variables

		GameData* gameData;
	public:
		Game(int windowWidth, int windowHeight);
		~Game();

		void askPlayerName();
		void startGame();
		bool setMap(std::string filename);
		
		// if connection established, randomly sets firstTurn and sends it to the opponent
		//bool sendFirstTurn();

		// if connection established, receives firstTurn packet
		//bool receiveFirstTurn();

		//bool hostGame(unsigned short port);
		//bool joinGame(sf::IpAddress hostIP, unsigned short port);
		

		//GUI methods
		void run();
	};
}