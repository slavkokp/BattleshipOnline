#include "pch.h"
#include "InputField.h"

namespace Battleship
{
	InputField::InputField(sf::Vector2f size, sf::Vector2f pos, std::string defaultString, sf::Color outlineColor)
	{
		this->isActive = false;

		this->inputShape.setPosition(pos);
		this->inputShape.setSize(size);
		this->inputShape.setOutlineThickness(-1.f);
		this->inputShape.setOutlineColor(outlineColor);
		this->inputShape.setFillColor(sf::Color::Color(139, 139, 139));

		this->textString = defaultString;
		this->displayedText = defaultString;

		this->inputText.setString(displayedText);
		this->inputText.setCharacterSize((int)size.y - 2);
		this->inputText.setFillColor(sf::Color::Black);
		this->inputText.setPosition(pos);
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

		//this->textString = this->inputText.getString();
		
		this->displayedText = this->inputText.getString();

		// if text backspace
		if (chr == 8)
		{
			if (displayedText.getSize() > 0)
			{
				size_t dispSize = displayedText.getSize();
				size_t wholeTextSize = textString.getSize();
				this->textString.erase(wholeTextSize-- - 1, 1);
				this->displayedText.erase(dispSize-- - 1, 1);
				if (dispSize != wholeTextSize)
				{
					displayedText = textString[wholeTextSize - dispSize - 1] + displayedText;
				}
				this->inputText.setString(displayedText);
			}
		}
		else
		{
			this->textString += chr;
			displayedText += chr;
			while (this->inputText.getGlobalBounds().width > this->inputShape.getGlobalBounds().width)
			{
				displayedText.erase(0, 1);
				this->inputText.setString(displayedText);
			}
			this->inputText.setString(displayedText);
		}
	}

	void InputField::updateCopy(sf::Event& event)
	{
		if (event.key.control && event.key.code == sf::Keyboard::C && this->isActive)
		{
			sf::Clipboard::setString(this->textString);
		}
	}

	void InputField::updatePaste(sf::Event& event)
	{
		if (event.key.control && event.key.code == sf::Keyboard::V && this->isActive)
		{
			this->setString(sf::Clipboard::getString());
			
		}
	}

	void InputField::setString(std::string str)
	{
		this->textString = str;
		this->displayedText = textString;
		this->inputText.setString(displayedText);
		while (this->inputText.getGlobalBounds().width > this->inputShape.getGlobalBounds().width)
		{
			displayedText.erase(0, 1);
			this->inputText.setString(displayedText);
		}
		this->inputText.setString(displayedText);
	}

	void InputField::setOutlineColor(sf::Color color)
	{
		this->inputShape.setOutlineColor(color);
	}

	void InputField::setPosition(sf::Vector2f pos)
	{
		this->inputShape.setPosition(pos);
		this->inputText.setPosition(pos);
	}

	void InputField::setFont(sf::Font& font)
	{
		this->inputText.setFont(font);
	}

	std::string InputField::getText()const
	{
		return this->textString;
	}

	bool InputField::getIsActive()const
	{
		return this->isActive;
	}

	sf::Vector2f InputField::getPosition()const
	{
		return this->inputShape.getPosition();
	}

	sf::FloatRect InputField::getGlobalBounds()const
	{
		return this->inputShape.getGlobalBounds();
	}
}