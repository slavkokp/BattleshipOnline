#pragma once
#include "MenuScreen.h"
#include "SignInScreen.h"

namespace Battleship
{
	class LoginScreen : public Screen
	{
	private:
		GameData* data;
		std::map<std::string, InputField*> inputs;
		std::map<std::string, Button*> buttons;
		std::map<std::string, sf::Text> labels;
		sf::Text errorMsg;
		bool error;
		bool multipleCallsProtection;

		sf::RectangleShape checkbox;
		sf::Text checkboxText;
		bool rememberMe;

		// private methods

		void initButtons(sf::Vector2f& windowSize, sf::Font& font);
		void initInputs(sf::Vector2f& windowSize, sf::Font& font);
		void initLabels(sf::Vector2f& windowSize, sf::Font& font);
		void initErrorMsg(sf::Vector2f& windowSize, sf::Font& font);
		void initCheckbox(sf::Vector2f& windowSize, sf::Font& font);

		void changeErrorMsgString(std::string str);
		void editRememberMe();
	public:
		LoginScreen(GameData* data);
		~LoginScreen();

		void updateButtonsFunction();
		void updateButtonsVisual();

		void updateCheckbox(sf::Mouse::Button& btn);
		bool login(pqxx::result& r);
		void update() override;
		void handleEvents() override;
		void render() override;
	};
}