#pragma once

namespace Battleship
{
	class InputField : public sf::Drawable
	{
	private:
		sf::RectangleShape inputShape;
		sf::Text inputText;
		sf::Font& font;
		sf::String textString;
		bool isActive;

		// private methods
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	public:
		InputField(sf::Vector2f size, sf::Vector2f pos, sf::Font& textFont, std::string defaultText = "", sf::Color outlineColor = sf::Color::Black);
		~InputField();

		void update(const sf::Vector2f& mousePos, sf::Mouse::Button& btn);
		void updateText(sf::Uint32& text);
		void updateCopy(sf::Event& event);
		void updatePaste(sf::Event& event);
		
		//setters

		void setString(std::string str);
		
		//getters

		std::string getText();
		bool getIsActive();
	};
}