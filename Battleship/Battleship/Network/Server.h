#pragma once

#include "Connection.h"

namespace Battleship
{
	class Server : public Connection
	{
	private:
		sf::TcpListener listener;
		sf::TcpSocket clientSocket;
	public:
		bool listen(unsigned short port);
		sf::TcpSocket& getSocket() override;
	};
}