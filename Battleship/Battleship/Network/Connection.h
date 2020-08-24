#pragma once

namespace Battleship
{
	class Connection
	{
	public:
		virtual ~Connection() {};
		virtual void dispose() = 0;
		virtual sf::TcpSocket& getSocket() = 0;
		virtual bool isReady(unsigned short port, sf::IpAddress serverAdress) = 0;
	};
}