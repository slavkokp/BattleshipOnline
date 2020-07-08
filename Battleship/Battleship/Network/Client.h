#pragma once

#include "Connection.h"

namespace Battleship
{
	class Client : public Connection
	{
	private:
		sf::TcpSocket socket;
	public:
		bool connect(sf::IpAddress serverAdress, unsigned short port);
		sf::TcpSocket& getSocket() override;
	};
}