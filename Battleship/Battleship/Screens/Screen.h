#pragma once

namespace Battleship
{
	class Screen
	{
	protected: 
		sf::Clock clock;
	public:
		virtual ~Screen() {};

		void restartClock();

		virtual void update() = 0;
		virtual void handleEvents() = 0;
		virtual void render() = 0;
	};
}