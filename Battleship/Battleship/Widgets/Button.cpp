#include "pch.h"
#include "Button.h"

namespace Battleship
{
	Button::Button(sf::Vector2f pos, std::string text, sf::Font font, int idleCharacterSize, 
					int activeCharacterSize, sf::Color pressedButtonTextColor, sf::Color color)
	{	
		this->currState = ButtonState::IDLE;
		this->font = font;
		this->activeCharacterSize = activeCharacterSize;
		this->idleCharacterSize = idleCharacterSize;
		this->textColor = color;
		this->pressedButtonTextColor = pressedButtonTextColor;

		this->shape.setPosition(pos);
		
		this->text.setFont(this->font);
		this->text.setCharacterSize(this->idleCharacterSize);
		this->text.setString(text);
		this->text.setPosition(shape.getPosition().x, shape.getPosition().y);
		this->text.setFillColor(textColor);

		//temporary shape settings
		this->shape.setSize(sf::Vector2f(this->text.getLocalBounds().width, this->text.getLocalBounds().height));
		this->shape.setFillColor(sf::Color::Transparent);
	}

	Button::~Button()
	{

	}

	void Button::setString(std::string str)
	{
		this->text.setString(str);
		this->shape.setSize(sf::Vector2f(this->text.getLocalBounds().width, this->text.getLocalBounds().height));
	}

	void Button::move(const sf::Vector2f& offset)
	{
		this->text.move(offset);
		this->shape.move(offset);
	}

	sf::FloatRect Button::getGlobalBounds()const
	{
		return this->text.getGlobalBounds();
	}

	void Button::update(const sf::Vector2f& mousePos)
	{
		if (this->shape.getGlobalBounds().contains(mousePos))
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				changeState(ButtonState::PRESSED);
				return;
			}
			changeState(ButtonState::HOVER);
			return;
		}
		changeState(ButtonState::IDLE);
	}

	void Button::updateAnimations()
	{
		switch (this->currState)
		{
		case Battleship::IDLE:
			if (this->text.getCharacterSize() == this->activeCharacterSize)
			{
				this->text.setCharacterSize(this->idleCharacterSize);
				this->shape.setSize(sf::Vector2f(this->text.getLocalBounds().width, this->text.getLocalBounds().height));
			}
			if (this->text.getOutlineColor() == this->textColor)
			{
				setTextColor(sf::Color::Color(), this->textColor);
			}
			break;
		case Battleship::HOVER:
			if (this->text.getOutlineColor() == this->textColor)
			{
				setTextColor(sf::Color::Color(), this->textColor);
			}
			this->text.setCharacterSize(this->activeCharacterSize);
			this->shape.setSize(sf::Vector2f(this->text.getLocalBounds().width, this->text.getLocalBounds().height));
			break;
		case Battleship::PRESSED:
			setTextColor(this->textColor, this->pressedButtonTextColor);
			break;
		}
	}

	void Button::changeState(const ButtonState& newState)
	{
		if (this->currState != newState)
		{
			this->currState = newState;
			updateAnimations();
		}
	}

	void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(this->shape, states);
		target.draw(this->text, states);
	}

	bool Button::isPressed()
	{
		if (this->currState == ButtonState::PRESSED)
		{
			return true;
		}
		return false;
	}

	void Button::setTextColor(sf::Color outlineColor, sf::Color& fillColor)
	{
		this->text.setOutlineColor(outlineColor);
		this->text.setFillColor(fillColor);
	}
}