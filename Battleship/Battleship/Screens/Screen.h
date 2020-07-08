#pragma once

namespace Battleship
{
	class Screen
	{
	public:
		virtual ~Screen() {};

		virtual void update() = 0;
		virtual void handleEvents() = 0;
		virtual void render() = 0;
	};
}