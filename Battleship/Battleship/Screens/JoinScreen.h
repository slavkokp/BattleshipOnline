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
		sf::Text connectionFailureMsg;
		bool connectionFailed;

		// private methods
		void initButtons(sf::Vector2f& windowSize);
		void initMessages(sf::Vector2f& windowSize, sf::Vector2f& size);
	public:
		JoinScreen(GameData* data);
		~JoinScreen();

		void updateButtonsVisual();
		void updateButtonsFunction();
		void update()override;
		void handleEvents()override;
		void render()override;
	};
}