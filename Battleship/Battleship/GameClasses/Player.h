#pragma once

#include <unordered_map>
#include "BattleshipMap.h"
#include "../Network/Server.h"
#include "../Network/Client.h"

namespace Battleship
{
	enum Row { A = 0, B, C, D, E, F, G, H, I, J };
	static std::unordered_map<char, Row> const RowTable = 
	{ {'A', Row::A}, {'B', Row::B}, {'C', Row::C}, {'D', Row::D}, {'E', Row::E}, 
	{'F', Row::F}, {'G', Row::G}, {'H', Row::H}, {'I', Row::I}, {'J', Row::J} };

	class Player
	{
	private:
		std::string name;
		BattleshipMap map;
		BattleshipMap mapClone;
		BattleshipMap opponentDummy;
		Connection* connection;
		bool firstTurn;
	public:
		Player(std::string name = "");
		~Player();

		void attack(Row row, int col, bool success); // row range: [1-10];
		std::pair<bool, std::string> takeDamage(Row row, int col, bool& won); // row range: [1-10];
		
		bool hostGame(unsigned short port);
		void joinGame();

		// if connection established, sends firstTurn packet, returns true when sent
		bool sendFirstTurn(sf::Packet& packet);

		// if connection established, receives firstTurn packet and sets first turn, returns true when recieved
		bool receiveFirstTurn(sf::Packet& packet);

		void printMaps()const;
		void prepareDisplayalbeMaps();

		bool setMap(std::string filename);
		bool setMap(BattleshipMap map);
		void setName(std::string name);
		void setFirstTurn(bool firstTurn);

		BattleshipMap& getMapClone();
		BattleshipMap& getOpponentDummy();
		BattleshipMap& getMap();
		Connection* getConnection();
		std::string getName()const;
		bool getFirstTurn()const;
		char getMapSymbByValue(Row val);
	};
}