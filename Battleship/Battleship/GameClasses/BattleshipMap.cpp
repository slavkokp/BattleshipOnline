#include "pch.h"
#include "BattleshipMap.h"

namespace Battleship
{
	BattleshipMap::BattleshipMap()
	{
		this->clear();
	}

	BattleshipMap::BattleshipMap(BattleshipMap& other)
	{
		for (size_t i = 0; i < MAP_SIZE; i++)
		{
			for (size_t j = 0; j < MAP_SIZE; j++)
			{
				this->map[i][j] = other.map[i][j];
			}
		}
		this->fieldsLeft = other.fieldsLeft;
	}

	void BattleshipMap::copy(BattleshipMap& other)
	{
		this->fieldsLeft = other.fieldsLeft;
		for (size_t i = 0; i < MAP_SIZE; i++)
		{
			for (size_t j = 0; j < MAP_SIZE; j++)
			{
				this->map[i][j] = other.map[i][j];
			}
		}
	}

	int BattleshipMap::getFieldsLeft()
	{
		return fieldsLeft;
	}

	int* BattleshipMap::operator[](int i)
	{
		return this->map[i];
	}

	void BattleshipMap::clear()
	{
		this->fieldsLeft = 0;
		for (size_t i = 0; i < MAP_SIZE; i++)
		{
			for (size_t j = 0; j < MAP_SIZE; j++)
			{
				this->map[i][j] = 0;
			}
		}
	}

	std::vector<sf::Vector2u> BattleshipMap::getShipsCoords()
	{
		std::vector<sf::Vector2u> res;
		unsigned int begin;
		int counter = 0;
		for (int i = 0; i < MAP_SIZE; i++)
		{
			for (int j = 0; j < MAP_SIZE; j++)
			{
				if (map[i][j])
				{
					begin = i * 10 + j;
					if (j < 9 && map[i][j + 1])
					{
						counter++;
						if (j < 8 && map[i][j + 2])
						{
							counter++;
							if (j < 7 && map[i][j + 3])
							{
								counter++;
							}
						}
						j += counter;
						res.push_back(sf::Vector2u(begin, i * 10 + j));
						counter = 0;
						continue;
					}
					if ((i < 1 || !map[i - 1][j]) && (i > 8 || !map[i + 1][j]))
					{
						begin = i * 10 + j;
						res.push_back(sf::Vector2u(begin, begin));
					}
				}
			}
		}
		for (int j = 0; j < MAP_SIZE; j++)
		{
			for (int i = 0; i < MAP_SIZE; i++)
			{
				if (map[i][j])
				{
					begin = i * 10 + j;
					if (i < 9 && map[i + 1][j])
					{
						counter++;
						if (i < 8 && map[i + 2][j])
						{
							counter++;
							if (i < 7 && map[i + 3][j])
							{
								counter++;
							}
						}
						i += counter;
						res.push_back(sf::Vector2u(begin, i * 10 + j));
						counter = 0;
					}
				}
			}
		}
		return res;
	}

	void BattleshipMap::setFieldsLeft(int value)
	{
		this->fieldsLeft = value;
	}

	bool BattleshipMap::readMapFromFile(std::string filePath)
	{
		std::ifstream inputFile;
		inputFile.open(filePath);
		for (int i = 0; i < MAP_SIZE; i++)
		{
			for (int j = 0; j < MAP_SIZE; j++)
			{
				inputFile >> map[i][j];
			}
		}
		inputFile.close();
		this->fieldsLeft = 20;
		return validateMap();
	}

	std::pair<bool, std::string> BattleshipMap::attack(int row, int col)
	{
		bool success = false;
		std::string message = "Missed";
		if (map[row][col] == 1)
		{
			fieldsLeft--;
			success = true;
			map[row][col] = Hit;
			if ((col < 9 && map[row][col + 1] == 1) || (col > 0 && map[row][col - 1] == 1) ||
				( row < 9 && map[row + 1][col] == 1) || (row > 0 && map[row - 1][col] == 1))
			{
				message = "Damaged";
			}
			else
			{
				message = "Destroyed";
			}
		}
		else if (map[row][col] != 0 && map[row][col] != 1)
		{
			message = "already attacked";
		}
		else
		{
			map[row][col] = Missed;
		}
		return std::make_pair(success, message);
	}

	bool BattleshipMap::validateMap()const
	{
		int counter = 0;
		for (int i = 0; i < MAP_SIZE; i++)
		{
			for (int j = 0; j < MAP_SIZE; j++)
			{
				if (map[i][j])
				{
					counter++;
				}
			}
		}
		if (counter != 20)
		{
			return false;
		}
		for (int i = 0; i < MAP_SIZE; i++)
		{
			for (int j = 0; j < MAP_SIZE; j++)
			{
				if (map[i][j])
				{
					if (i > 0 && j > 0 && map[i - 1][j - 1])
					{
						return false;
					}
					if (i > 0 && j < 9 && map[i - 1][j + 1])
					{
						return false;
					}
					if (i < 9 && j > 0 && map[i + 1][j - 1])
					{
						return false;
					}
					if (i < 9 && j < 9 && map[i + 1][j + 1])
					{
						return false;
					}
				}
			}
		}
		counter = 0;
		int shipsCount[4] {0, 0, 0, 0};
		for (int i = 0; i < MAP_SIZE; i++)
		{
			for (int j = 0; j < MAP_SIZE; j++)
			{
				if (map[i][j])
				{
					if (j < 9 && map[i][j + 1])
					{
						counter++;
						if (j < 8 && map[i][j + 2])
						{
							counter++;
							if (j < 7 && map[i][j + 3])
							{
								counter++;
							}
						}
						shipsCount[counter]++;
						j += counter;
						counter = 0;
					}
				}
			}
		}
		for (int j = 0; j < MAP_SIZE; j++)
		{
			for (int i = 0; i < MAP_SIZE; i++)
			{
				if (map[i][j])
				{
					if (i < 9 && map[i + 1][j])
					{
						counter++;
						if (i < 8 && map[i + 2][j])
						{
							counter++;
							if (i < 7 && map[i + 3][j])
							{
								counter++;
							}
						}
						shipsCount[counter]++;
						i += counter;
						counter = 0;
					}
				}
			}
		}
		if (shipsCount[1] == 3 && shipsCount[2] == 2 && shipsCount[3] == 1)
		{
			return true;
		}
		return false;
	}
}