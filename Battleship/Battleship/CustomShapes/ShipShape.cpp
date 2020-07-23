#include "pch.h"
#include "ShipShape.h"

namespace Battleship
{
	ShipShape::ShipShape(int abstractSize, sf::Vector2f cellSize, sf::Color fillColor, sf::Color outlineColor, float outlineThickness)
	{
		this->shipShape.setSize(sf::Vector2f(cellSize.x, cellSize.y * abstractSize));
		this->shipShape.setFillColor(fillColor);
		this->shipShape.setOutlineColor(outlineColor);
		this->shipShape.setOutlineThickness(outlineThickness);
	}

	ShipShape::~ShipShape()
	{
	}

	void ShipShape::move(float offsetX, float offsetY)
	{
		this->move(sf::Vector2f(offsetX, offsetY));
	}

	void ShipShape::move(const sf::Vector2f& offset)
	{
		this->shipShape.move(offset);
	}

	void ShipShape::rotate()
	{
		if (this->shipShape.getRotation() != 0.f)
		{
			this->shipShape.rotate(90.f);
			this->shipShape.move(0.f, -this->shipShape.getSize().x);
		}
		else
		{
			this->shipShape.move(0.f, this->shipShape.getSize().x);
			this->shipShape.rotate(270.f);
		}
	}

	void ShipShape::setSize(const sf::Vector2f& size)
	{
		this->shipShape.setSize(size);
	}

	void ShipShape::setPosition(float x, float y)
	{
		this->setPosition(sf::Vector2f(x, y));
	}

	void ShipShape::setPosition(const sf::Vector2f& position)
	{
		if (this->getRotation() == 0.f)
		{
			this->shipShape.setPosition(position);
		}
		else
		{
			this->shipShape.setPosition(position.x, position.y + this->shipShape.getSize().x);
		}
	}

	void ShipShape::resetRotation()
	{
		this->shipShape.setRotation(0.f);
	}

	void ShipShape::setOutlineThickness(float thickness)
	{
		this->shipShape.setOutlineThickness(thickness);
	}

	void ShipShape::setFillColor(sf::Color color)
	{
		this->shipShape.setFillColor(color);
	}

	void ShipShape::setOutlineColor(sf::Color color)
	{
		this->shipShape.setOutlineColor(color);
	}

	const sf::Vector2f ShipShape::getSize() const
	{
		sf::FloatRect bounds = this->shipShape.getGlobalBounds();
		return sf::Vector2f(bounds.width, bounds.height);
	}

	const sf::Vector2f ShipShape::getPosition() const
	{
		sf::FloatRect bounds = this->shipShape.getGlobalBounds();
		return sf::Vector2f(bounds.left, bounds.top);
	}

	float ShipShape::getRotation() const
	{
		return this->shipShape.getRotation();
	}

	sf::FloatRect ShipShape::getGlobalBounds() const
	{
		return this->shipShape.getGlobalBounds();
	}

	void ShipShape::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(this->shipShape, states);
	}
}