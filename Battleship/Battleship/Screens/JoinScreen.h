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
		
		sf::Packet firstTurnPacket;
		sf::Clock connectionTimer;
		bool connectionFailed;
		bool connecting;

		// private methods
		void initButtons(sf::Vector2f& windowSize);
		void initMessages(sf::Vector2f& windowSize, sf::Vector2f& size);
	public:
		JoinScreen(GameData* data);
		~JoinScreen();

		void updateConnecting();
		void updateButtonsVisual();
		void updateButtonsFunction();
		void update()override;
		void handleEvents()override;
		void render()override;
	};
}