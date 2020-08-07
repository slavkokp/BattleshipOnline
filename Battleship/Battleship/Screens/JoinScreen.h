#pragma once

#include "GameScreen.h"
#include "../Widgets/InputField.h"

namespace Battleship
{
	class JoinScreen : public Screen
	{
	private:
		GameData* data;
		std::map<std::string, Button*> buttons;
		InputField* inputField;
		sf::Text inputMsg;
		sf::Text connectionMsg;
		
		std::string nameOfOpponent;
		sf::Packet firstTurnPacket;
		sf::Packet namePacket;
		sf::Clock connectionTimer;
		bool connectionFailed;
		bool connecting;
		bool sending;
		bool receiving;

		// private methods
		void initButtons(sf::Vector2f& windowSize);
		void initMessages(sf::Vector2f& windowSize, sf::Vector2f& size);
		void initInputField(sf::Vector2f& windowSize, sf::Vector2f& size);
	public:
		JoinScreen(GameData* data);
		~JoinScreen();

		bool sendName();
		bool receiveName();
		void updateConnecting();
		void updateButtonsVisual();
		void updateButtonsFunction();
		void update()override;
		void handleEvents()override;
		void render()override;
	};
}