#pragma once

#include "Connection.h"

namespace Battleship
{
	class Client : public Connection
	{
	private:
		sf::TcpSocket socket;
	public:
		Client();

		void dispose()override;
		sf::TcpSocket& getSocket() override;

		//use this to connect
		bool isReady(unsigned short port, sf::IpAddress serverAdress) override;
	};
}