#pragma once
#include "LoginScreen.h"

namespace Battleship
{
	class SignInScreen : public Screen
	{
	private:
		GameData* data;
		std::map<std::string, InputField*> inputs;
		std::map<std::string, Button*> buttons;
		std::map<std::string, sf::Text> labels;
		sf::Text errorMsg;
		bool error;
		bool multipleCallsProtection;

		// private methods

		void initInputs(sf::Vector2f& windowSize, sf::Font& font);
		void initLabels(sf::Vector2f& windowSize, sf::Font& font);
		void initButtons(sf::Vector2f& windowSize, sf::Font& font);
		void initErrorMsg(sf::Vector2f& windowSize, sf::Font& font);

		void changeErrorMsgString(std::string str);
		bool parseInput(std::map<std::string, std::string>& result, std::string& errorString);
	public:
		SignInScreen(GameData* data);
		~SignInScreen();

		bool signIn();
		void updateButtonsFunction();
		void updateButtonsVisual();
		void update() override;
		void handleEvents() override;
		void render() override;
	};
}