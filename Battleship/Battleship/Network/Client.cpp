#include "Client.h"


namespace Battleship
{
	bool Client::connect(sf::IpAddress serverAdress, unsigned short port)
	{
		sf::Socket::Status status = socket.connect(serverAdress, port);
		if (status != sf::Socket::Done)
		{
			std::cout << "cannot connect to " << serverAdress.toString() << ':' << port << std::endl;
			return false;
		}
		return true;
	}
	sf::TcpSocket& Client::getSocket()
	{
		return socket;
	}
}