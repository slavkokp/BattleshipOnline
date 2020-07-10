#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

namespace Battleship
{
	class InputManager
	{
	private:
		sf::Vector2f mousePosView;
	public:
		void updateMousePosView(const sf::RenderWindow& window);
		sf::Vector2f& getMousePosView();
	};
}