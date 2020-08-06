#include "pch.h"
#include "Game.h"

namespace Battleship
{
	Game::Game(int windowWidth, int windowHeight) : gameData(new GameData())
	{
		this->gameData->window.create(sf::VideoMode(windowWidth, windowHeight), "Battleship game", sf::Style::Default);
		this->gameData->screenManager.addScreen(new LoginScreen(this->gameData), false);
	}

	Game::~Game()
	{
		delete this->gameData;
	}

	void Game::run()
	{
		while (this->gameData->window.isOpen())
		{
			this->gameData->screenManager.handleScreenSwitches();
			this->gameData->screenManager.getCurrentScreen().handleEvents();
			this->gameData->screenManager.getCurrentScreen().update();
			this->gameData->screenManager.getCurrentScreen().render();
		}
	}
}