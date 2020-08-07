#pragma once

#include "JoinScreen.h"

namespace Battleship
{
	class HostScreen : public Screen
	{
	private:
		GameData* data;
		Button* exitButton;
		InputField* IPshowcase;
		sf::Text showcaseLabel;
		sf::Text waitingMsg;
		sf::Packet firstTurnPacket;

		unsigned short serverPort;
		std::string nameOfOpponent;
		sf::Packet namePacket;
		bool sending;
		bool receiving;
		//private methods

		void initIPshowcase(sf::Vector2f& windowSize, sf::Vector2f& showcaseSize);
		void initShowcaseLabel(sf::Vector2f& windowSize, sf::Vector2f& showcaseSize);
		void initWaitingMsg(sf::Vector2f& windowSize, sf::Vector2f& showcaseSize);
	public:
		HostScreen(GameData* data);
		~HostScreen();
		
		bool sendName();
		bool receiveName();
		void updateButtonVisual();
		void updateButtonFunction();
		void update() override;
		void handleEvents() override;
		void render() override;
	};
}