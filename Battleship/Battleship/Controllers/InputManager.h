#pragma once

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