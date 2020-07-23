#include "pch.h"
#include "Client.h"


namespace Battleship
{
	Client::Client()
	{
		this->socket.setBlocking(false);
	}

	void Client::dispose()
	{
		this->socket.disconnect();
	}

	sf::TcpSocket& Client::getSocket()
	{
		return this->socket;
	}

	bool Client::isReady(unsigned short port, sf::IpAddress serverAdress)
	{
		if (this->socket.getRemotePort() == 0)
		{
			this->socket.connect(serverAdress, port);
			return false;
		}
		return true;
	}
}