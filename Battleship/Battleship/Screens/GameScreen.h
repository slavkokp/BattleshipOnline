#pragma once

#include "../Controllers/ScreenManager.h"
#include "../CustomShapes/MapShape.h"
namespace Battleship
{
	class GameScreen : public Screen
	{
	private:
		GameData* data;
		MapShape playerClone;
		MapShape opponentsDummy;
		sf::Text gameStatusMsg;
		sf::Packet packet;
		sf::TcpSocket& opponentSocket;
		std::map<std::string, Button*> buttons;
		bool drawOKbtn;
		bool waitingForPlayer;

		// 101 for the first iteration in update loop to set 100 index 
		// to true so that we can wait for player to choose something
		bool cellStatusArr[101];
		int attackedCellIndex;
		bool missed;
		bool won;
		std::string msg;

		bool myTurn;
		bool startSendingInput;
		bool startRecievingAttackRes;
		bool startRecievingInput;
		bool startSendingAttackRes;

		// private methods

		void initButtons(sf::Vector2f& windowSize);
		void initGameStatusMsg(sf::Vector2f& windowSize);
		void initPlayerClone();
		void initOpponentsDummy();
	public:
		GameScreen(GameData* data);
		~GameScreen();

		void setGameStatusMsgString(std::string msg);
		void sendInput();
		void sendAttackRes();
		void receiveInput();
		void receiveAttackRes();
		void attack(int cellNum);
		void updateConnectionStatus(sf::Socket::Status& status);
		void updateButtonsVisual();
		void updateButtonsFunction();
		void update()override;
		void handleEvents()override;
		void render()override;
	};
}