#include "pch.h"
#include "Server.h"

namespace Battleship
{
	bool Server::listen(unsigned short port)
	{
		if (listener.listen(port) != sf::Socket::Done)
		{
			std::cout << "Port error" << std::endl;
			return false;
		}
		if (listener.accept(clientSocket) != sf::Socket::Done)
		{
			std::cout << "no connection attempts" << std::endl;
			return false;
		}
		return true;
	}

	sf::TcpSocket& Server::getSocket()
	{
		return clientSocket;
	}
}