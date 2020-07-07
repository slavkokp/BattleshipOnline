#pragma once

#include <unordered_map>
#include "Server.h"
#include "Client.h"
#include "BattleshipMap.h"

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
	public:
		Player(std::string name = "");
		void attack(Row row, int col, bool success); // row range: [1-10];
		std::pair<bool, std::string> takeDamage(Row row, int col, bool& won); // row range: [1-10];
		bool setMap(std::string filename);
		void setName(std::string name);
		void printMaps()const;
		void prepareDisplayalbeMaps();
		BattleshipMap& getMapClone();
		BattleshipMap& getOpponentDummy();
		std::string getName()const;
	};
}