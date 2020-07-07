#include "Game.h"

int main()
{
	unsigned int time_ui = static_cast<unsigned int>(time(NULL));
	srand(time_ui);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	Battleship::Game game(800, 400);
	game.run();
	/*game.askPlayerName();
	std::cout << "Enter 1 to host or 2 to join: ";
	int i;
	std::cin >> i;
	bool isMapValid;
	if (i == 1)
	{
		std::cout << "Your IP: " << sf::IpAddress::getPublicAddress() << std::endl;
		game.hostGame(55555);
		game.sendFirstTurn();
		isMapValid = game.setMap("BattleshipMapInput2.txt");
	}
	else if (i == 2)
	{
		std::cout << "Enter host IP: " << std::endl;
		std::string IP;
		std::cin >> IP;
		sf::IpAddress address(IP);
		game.joinGame(address, 55555);
		game.receiveFirstTurn();
		isMapValid = game.setMap("BattleshipMapInput.txt");
	}
	else
	{
		std::cout << "wrong input" << std::endl;
		return 0;
	}
	if (!isMapValid)
	{
		std::cout << "invalid_map" << std::endl;
		system("pause");
		return -1;
	}
	std::cin.get();
	game.startGame();
	system("pause");*/
	return 0;
}