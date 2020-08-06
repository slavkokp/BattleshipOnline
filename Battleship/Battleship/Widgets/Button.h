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

	class Button : public sf::Drawable
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
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	public:
		Button(sf::Vector2f pos, std::string text, sf::Font font, int idleCharacterSize, 
			int activeCharacterSize, sf::Color pressedButtonTextColor, sf::Color color = sf::Color::White);
		~Button();

		void setString(std::string str);
		void move(const sf::Vector2f& offset);
		
		sf::FloatRect getGlobalBounds()const;

		bool isPressed();
		void update(const sf::Vector2f& mousePos);
	};
}