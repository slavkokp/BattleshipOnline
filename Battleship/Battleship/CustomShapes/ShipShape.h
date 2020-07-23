#pragma once

#include<SFML/Graphics.hpp>

namespace Battleship
{
	class ShipShape : public sf::Drawable, public sf::Transformable
	{
	private:
		sf::RectangleShape shipShape;

		// private methods

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	public:
		// abstractSize must be in [1-4] like in Battleship game
		ShipShape(int abstractSize, sf::Vector2f cellSize, sf::Color fillColor = sf::Color::Transparent, 
					sf::Color outlineColor = sf::Color::Black, float outlineThickness = -1.f);
		~ShipShape();

		// transform modificators

		void move(float offsetX, float offsetY);
		void move(const sf::Vector2f& offset);

		// switches rotation depending on current
		// if current rotation is 0.f sets it to 270.f and vice versa
		// this also moves the sprite so that it's center will be in mouse cursor position.
		void rotate();

		// setters

		void setSize(const sf::Vector2f& size);
		void setPosition(float x, float y);

		// sets top left corner pos to \position
		void setPosition(const sf::Vector2f& position);
		void setOutlineThickness(float thickness);
		void setFillColor(sf::Color color);
		void setOutlineColor(sf::Color color);
		
		// sets rotation to default
		void resetRotation();

		// getters

		// returns width and height considering rotation
		const sf::Vector2f getSize() const;
		
		// always returns coords of top left point, even if the shape is rotated 
		const sf::Vector2f getPosition() const;
		
		sf::FloatRect getGlobalBounds()const;
		float getRotation() const;
	};
}