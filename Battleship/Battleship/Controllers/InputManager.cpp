#include "pch.h"
#include "InputManager.h"

namespace Battleship
{
	void InputManager::updateMousePosView(const sf::RenderWindow& window)
	{
		this->mousePosView = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	}

	sf::Vector2f& InputManager::getMousePosView()
	{
		return this->mousePosView;
	}
}