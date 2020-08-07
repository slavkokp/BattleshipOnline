#pragma once

namespace Battleship
{
	class MatchHistoryLine : public sf::Drawable, public sf::Transformable
	{
	private:
		sf::RectangleShape container;
		std::map<std::string, sf::Text> columnTexts;
		sf::Font& font;
		//private methods

		void initColumnTexts(pqxx::row& row, std::string playerName, bool isHeader);
		void initContainer(sf::Vector2f& pos, sf::Vector2f& size);

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	public:
		MatchHistoryLine(sf::Vector2f pos, sf::Vector2f size, pqxx::row* row, sf::Font& font, std::string& playerName, bool isHeader);
		~MatchHistoryLine();

		void setOutlineThickness(float thickness);
		void setFillColor(sf::Color color);
		void setFontColor(sf::Color color);
		void setOutlineColor(sf::Color color);

		sf::FloatRect getGlobalBounds()const;
	};
}