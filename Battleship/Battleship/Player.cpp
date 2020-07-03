#include "Player.h"

namespace Battleship
{
	Player::Player(std::string name): name(name), opponentMapClonePtr(nullptr) {}

	Player::Player(BattleshipMap* opponentMap, std::string name) : name(name), opponentMapClonePtr(opponentMap) {};

	std::string Player::getName()const
	{
		return name;
	}

	std::pair<bool, std::string> Player::attack(Row row, int col, bool& wonGame)
	{
		auto res = (*opponentMapClonePtr).attack((int)row, col);
		this->opponentDummy[(int)row][col] = res.first ? Hit : Missed;
		wonGame = (*opponentMapClonePtr).getFieldsLeft() == 0 ? true : false;
		return res;
	}
	void Player::setMap(std::string filename)
	{
		this->map.readMapFromFile(filename);
		this->mapClone = BattleshipMap(this->map);
	}
	void Player::setOpponentMap(BattleshipMap& map)
	{
		this->opponentMapClonePtr = &map;
	}
	void Player::printInterface()const
	{
		std::cout << "Opponent's map\t\tYour map" << std::endl;
		BattleshipMap::printMapsInRow(opponentDummy, mapClone);
		std::cout << "Enter position to be attacked: ";
	}
	BattleshipMap& Player::getMapClone()
	{
		return this->mapClone;
	}
}