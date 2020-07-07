#pragma once
#include <SFML/Network.hpp>

namespace Battleship
{
	class Connection
	{
	public:
		virtual sf::TcpSocket& getSocket() = 0;
	};
}