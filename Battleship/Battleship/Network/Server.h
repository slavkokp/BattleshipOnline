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
		Server();

		void dispose()override;
		bool listen(unsigned short port);
		sf::TcpSocket& getSocket() override;
		bool isReady(unsigned short port, sf::IpAddress serverAdress) override;
	};
}