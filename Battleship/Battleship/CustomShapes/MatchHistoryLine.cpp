#include "pch.h"
#include "MatchHistoryLine.h"

namespace Battleship
{
	MatchHistoryLine::MatchHistoryLine(sf::Vector2f pos, sf::Vector2f size, pqxx::row* row, sf::Font& font, std::string& playerName, bool isHeader) : font(font)
	{
		this->initContainer(pos, size);
		this->initColumnTexts(*row, playerName, isHeader);
	}

	MatchHistoryLine::~MatchHistoryLine()
	{
	}

	void MatchHistoryLine::initColumnTexts(pqxx::row& row, std::string playerName, bool isHeader)
	{
		std::string columnNames[]{ "winner", "loser", "date", "" };
		
		if (row[0].c_str() == playerName)
		{
			columnNames[3] = "win";
		}
		else
		{
			columnNames[3] = "loss";
		}

		sf::FloatRect contBounds = this->container.getGlobalBounds();
		for (int i = 0; i < 4; i++)
		{
			this->columnTexts[columnNames[i]].setCharacterSize((int)container.getSize().y - 2);
			this->columnTexts[columnNames[i]].setFillColor(sf::Color::White);
			this->columnTexts[columnNames[i]].setFont(font);
		}
		this->columnTexts[columnNames[3]].setFillColor(columnNames[3] == "win" ? sf::Color::Green : sf::Color::Red);

		if (isHeader)
		{
			this->columnTexts[columnNames[0]].setString("Winner");
			this->columnTexts[columnNames[1]].setString("Loser");
			this->columnTexts[columnNames[2]].setString("Date");
			this->columnTexts[columnNames[3]].setString("Result");
			this->columnTexts[columnNames[3]].setFillColor(sf::Color::White);
		}
		else
		{
			for (int i = 0; i < 3; i++)
			{
				this->columnTexts[columnNames[i]].setString(row[i].c_str());
			}
			this->columnTexts[columnNames[3]].setString(columnNames[3]);
		}
		
		this->columnTexts[columnNames[3]].setPosition(contBounds.left + contBounds.width - this->columnTexts[columnNames[3]].getGlobalBounds().width - 2.f, contBounds.top);
		this->columnTexts[columnNames[2]].setPosition(this->columnTexts[columnNames[3]].getGlobalBounds().left - this->columnTexts[columnNames[2]].getGlobalBounds().width - 10.f, contBounds.top);
		this->columnTexts[columnNames[1]].setPosition((this->columnTexts[columnNames[2]].getGlobalBounds().left + contBounds.left) / 2.f, contBounds.top);
		this->columnTexts[columnNames[0]].setPosition(contBounds.left + 2.f, contBounds.top);
		
	}

	void MatchHistoryLine::initContainer(sf::Vector2f& pos, sf::Vector2f& size)
	{
		this->container.setSize(size);
		this->container.setPosition(pos);
		this->container.setOutlineThickness(-1.f);
		this->container.setFillColor(sf::Color(50, 50, 50));
		this->container.setOutlineColor(sf::Color::White);
	}

	void MatchHistoryLine::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(this->container, states);
		for (auto& it : columnTexts)
		{
			target.draw(it.second, states);
		}
	}

	void MatchHistoryLine::setOutlineThickness(float thickness)
	{
		this->container.setOutlineThickness(thickness);
	}

	void MatchHistoryLine::setFillColor(sf::Color color)
	{
		this->container.setFillColor(color);
	}

	void MatchHistoryLine::setFontColor(sf::Color color)
	{
		for (auto& it : columnTexts)
		{
			it.second.setFillColor(color);
		}
	}

	void MatchHistoryLine::setOutlineColor(sf::Color color)
	{
		this->container.setOutlineColor(color);
	}

	sf::FloatRect MatchHistoryLine::getGlobalBounds()const
	{
		return container.getGlobalBounds();
	}
}