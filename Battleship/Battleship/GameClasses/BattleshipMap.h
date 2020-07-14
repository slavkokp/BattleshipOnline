#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <SFML/System/Vector2.hpp>

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
		
		std::pair<bool, std::string> attack(int row, int col);
		int getFieldsLeft();
		int* operator[](int i);

		// call this for valid map only
		std::vector<sf::Vector2u> getShipsCoords();


		void clear();
		void copy(BattleshipMap& other);
		bool readMapFromFile(std::string filePath);
		bool validateMap()const;
		
		static void printMap(const BattleshipMap& map);
		static void printMapsInRow(const BattleshipMap& map1, const BattleshipMap& map2);
	};
}