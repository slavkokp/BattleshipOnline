#pragma once

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

		void setFieldsLeft(int value);

		void clear();
		void copy(BattleshipMap& other);
		bool readMapFromFile(std::string filePath);
		bool validateMap()const;
	};
}