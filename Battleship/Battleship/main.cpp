#include <cctype>
#include "Player.h"
#define ZAEBIS 0

using namespace Battleship;

namespace Battleship
{
	std::pair<Row, int> parseInput(std::string input, bool& isInputValid)
	{
		std::pair<Row, int> res;
		auto it = RowTable.find(std::toupper(input[0]));
		if (it != RowTable.end())
		{
			res.first = it->second;
		}
		else
		{
			isInputValid = false;
			return res;
		}
		res.second = std::stoi(input.substr(1)) - 1;
		if (res.second <= 0 || res.second > 10)
		{
			isInputValid = false;
			return res;
		}
		isInputValid = true;
		return res;
	}
}

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	Player slava("slava");
	slava.setMap("BattleshipMapInput.txt");
	Player dymok("dymok");
	dymok.setMap("BattleshipMapInput.txt");
	dymok.setOpponentMap(slava.getMapClone());
	slava.setOpponentMap(dymok.getMapClone());
	int turn = 0;
	std::string input;
	Player* currPlayer;
	bool won = false;
	std::pair<Row, int> parsedInput;
	std::pair<bool, std::string> attackRes;
	bool isInputValid;
	while (true)
	{
		if (turn % 2)
		{
			currPlayer = &dymok;
		}
		else
		{
			currPlayer = &slava;
		}
		(*currPlayer).printInterface();
		std::getline(std::cin, input);
		parsedInput = parseInput(input, isInputValid);
		if (!isInputValid)
		{
			std::cout << "Invalid input, try once more." << std::endl;
			system("pause");
			system("cls");
			continue;
		}
		attackRes = currPlayer->attack(parsedInput.first, parsedInput.second, won);
		if (attackRes.first)
		{
			std::cout << "Successful attack! You " << attackRes.second << " enemy ship!" << std::endl;
		}
		else
		{
			std::cout << "You " << attackRes.second << std::endl;
		}
		if (won)
		{
			std::cout << currPlayer->getName() << " won!!!" << std::endl;
			break;
		}
		turn++;
		system("pause");
		system("CLS");
	}
	system("pause");
	return ZAEBIS;
}