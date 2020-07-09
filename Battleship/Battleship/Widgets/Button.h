#pragma once

#include <SFML/Graphics.hpp>

namespace Battleship
{
	enum ButtonState
	{
		IDLE = 1,
		HOVER,
		PRESSED,
	};

	class Button
	{
	private:
		ButtonState currState;

		sf::RectangleShape shape;
		sf::Font font;
		sf::Text text;

		int activeCharacterSize;
		int idleCharacterSize;
		sf::Color textColor;
		sf::Color pressedButtonTextColor;

		//private methods

		void setTextColor(sf::Color outlineColor, sf::Color& fillColor);
	public:
		Button(sf::Vector2f pos, std::string text, sf::Font font, int idleCharacterSize, 
			int activeCharacterSize, sf::Color pressedButtonTextColor, sf::Color color = sf::Color::White);
		~Button();

		bool isPressed();
		void render(sf::RenderTarget& renderTarget);
		void update(const sf::Vector2f& mousePos);
	};
}