#include "pch.h"
#include "Server.h"

namespace Battleship
{
	Server::Server()
	{
		this->clientSocket.setBlocking(false);
		this->listener.setBlocking(false);
		
	}

	void Server::dispose()
	{
		this->clientSocket.disconnect();
		this->listener.close();
	}

	bool Server::listen(unsigned short port)
	{
		if (this->listener.listen(port) != sf::Socket::Done)
		{
			std::cerr << "Port error\n";
			return false;
		}
		return true;
	}

	sf::TcpSocket& Server::getSocket()
	{
		return this->clientSocket;
	}

	bool Server::isReady(unsigned short port, sf::IpAddress serverAdress)
	{
		if (this->listener.accept(this->clientSocket) != sf::Socket::Done)
		{
			return false;
		}
		return true;
	}
}