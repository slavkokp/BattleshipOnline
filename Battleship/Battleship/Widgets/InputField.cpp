#include "pch.h"
#include "InputField.h"

namespace Battleship
{
	InputField::InputField(sf::Vector2f size, sf::Vector2f pos, sf::Font& textFont, std::string defaultString, sf::Color outlineColor) : font(textFont)
	{
		this->isActive = false;

		this->inputShape.setPosition(pos);
		this->inputShape.setSize(size);
		this->inputShape.setOutlineThickness(-1.f);
		this->inputShape.setOutlineColor(outlineColor);
		this->inputShape.setFillColor(sf::Color::Color(139, 139, 139));

		this->textString = defaultString;

		this->inputText.setString(textString);
		this->inputText.setCharacterSize((int)size.y - 2);
		this->inputText.setFont(this->font);
		this->inputText.setFillColor(sf::Color::Black);
		this->inputText.setPosition(pos.x, pos.y);
	}

	InputField::~InputField()
	{
	}

	void InputField::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(this->inputShape, states);
		target.draw(this->inputText, states);
	}

	void InputField::update(const sf::Vector2f& mousePos, sf::Mouse::Button& btn)
	{
		if (btn == sf::Mouse::Left && this->inputShape.getGlobalBounds().contains(mousePos))
		{
			this->isActive = true;
			this->inputShape.setFillColor(sf::Color::White);
		}
		else
		{
			this->isActive = false;
			this->inputShape.setFillColor(sf::Color::Color(139, 139, 139));
		}
	}

	void InputField::updateText(sf::Uint32& chr)
	{
		if (!this->isActive)
		{
			return;
		}

		this->textString = this->inputText.getString();
		
		// if text backspace
		if (chr == 8)
		{
			this->inputText.setString(this->textString.substring(0, textString.getSize() - 1));
		}
		else
		{
			this->inputText.setString(this->textString + chr);
		}
	}

	void InputField::updateCopy(sf::Event& event)
	{
		if (event.key.control && event.key.code == sf::Keyboard::C && this->isActive)
		{
			sf::Clipboard::setString(this->inputText.getString());
		}
	}

	void InputField::updatePaste(sf::Event& event)
	{
		if (event.key.control && event.key.code == sf::Keyboard::V && this->isActive)
		{
			this->inputText.setString(sf::Clipboard::getString());
		}
	}

	void InputField::setString(std::string str)
	{
		this->inputText.setString(str);
	}

	std::string InputField::getText()
	{
		return this->inputText.getString();
	}

	bool InputField::getIsActive()
	{
		return this->isActive;
	}
}