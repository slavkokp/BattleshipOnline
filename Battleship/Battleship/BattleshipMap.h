#pragma once

#include <iostream>
#include <string>
#include <fstream>

namespace Battleship
{
	enum Action { Hit = 2, Missed };

	class BattleshipMap
	{
	private:
		static const unsigned int MAP_SIZE = 10;
		int map[MAP_SIZE][MAP_SIZE];
		int fieldsLeft;
	public:
		BattleshipMap();
		BattleshipMap(BattleshipMap& other);
		bool validateMap()const;
		void printMap();
		static void printMapsInRow(const BattleshipMap& map1, const BattleshipMap& map2);
		int getFieldsLeft();
		int* operator[](int i);
		void readMapFromFile(std::string filePath);
		std::pair<bool, std::string> attack(int row, int col);
	};
}