#include "NetworkInterface.h"
#include "ClientManager.h"
#include "GlobalValues.h"

void NetworkManager::Init()
{
	if (listener.listen(LISTENER_PORT) != sf::Socket::Status::Done) // Comprbar puerto valido
	{
		std::cerr << "Cannot Listen the port.\nExiting execution with code -1." << std::endl;
		return;
	}
	selector.add(listener);
}

void NetworkManager::CheckConnections()
{

	if (selector.wait())
	{
		if (selector.isReady(listener))
		{
			// RegisterNewUserConnection();

			// Aceptamos la nueva conexión
			Client* newClient = new Client(1, new sf::TcpSocket());
			int id;

			if (listener.accept(*newClient->GetSocket()) == sf::Socket::Status::Done)
			{
				newClient->GetSocket()->connect( *newClient->GetSocket()->getRemoteAddress(), 55010);
				std::cout << "PORFAVOR ALEX MIRAME ESTE PUERTO: " << newClient->GetSocket()->getRemotePort() << std::endl;
				newClient->GetSocket()->setBlocking(false); // Desbloqueamos el socket
				selector.add(*newClient->GetSocket());
				CLIENT_MANAGER.AddClient(newClient);
			}
			else
			{
				std::cout << "Intento de connexion no valido" << std::endl;
				delete newClient;
			}

		}
		else
		{
			for (Client* client : CLIENT_MANAGER.GetClients())
			{
				std::cout << CLIENT_MANAGER.GetClients().size() << std::endl;

				if (selector.isReady(*client->GetSocket()))
				{
					std::cout << "HEMOS SALIDO 2 VECES -- DE -- PARI -- " << std::endl;

					sf::Packet packet;
					if (client->GetSocket()->receive(packet) == sf::Socket::Status::Done)
					{
						std::cout << "PARA BAILAR LA BAMBA" << std::endl;

						std::string string;
						packet >> string;
						std::cout << "INFO: " << string << std::endl;
						packet.clear();
					}
					else if (client->GetSocket()->receive(packet) == sf::Socket::Status::Disconnected)
					{
						std::cout << "ME HE DESCONECTADOOOOOOOOOOOOOOOOO" << std::endl;
						client->GetSocket()->disconnect();
						selector.remove(*client->GetSocket());

						std::cout << CLIENT_MANAGER.GetClients().size() << std::endl;
					}
				}
			}
		}
	}	
}

void NetworkManager::RegisterNewUserConnection(Client* _newClient)
{	
	if (listener.accept(*_newClient->GetSocket()) == sf::Socket::Status::Done) // Añadir nuevo cliente HANDSHAKE
	{
		_newClient->GetSocket()->setBlocking(false); // Desbloqueamos el socket
		selector.add(*_newClient->GetSocket());

		std::cout << "Nueva conexion establecida --> " << _newClient->GetIP() << ":" << _newClient->GetSocket()->getRemotePort() << std::endl;
		CLIENT_MANAGER.AddClient(_newClient);
	}
}

void NetworkManager::SendData(sf::TcpSocket& _clientSocket, sf::Packet& _packet)
{
	sf::Socket::Status status = _clientSocket.send(_packet);

	std::cout << ", Port: " << _clientSocket.getRemotePort() << std::endl;
	if (status != sf::Socket::Status::Done)
	{
		std::cerr << "Error al enviar el paquete al cliente. Estado: ";

		switch (status)
		{
		case sf::Socket::Status::NotReady:
			std::cerr << "NotReady" << std::endl;
			break;
		case sf::Socket::Status::Partial:
			std::cerr << "Partial" << std::endl;
			break;
		case sf::Socket::Status::Disconnected:
			std::cerr << "Disconnected" << std::endl;
			break;
		case sf::Socket::Status::Error:
			std::cerr << "Error" << std::endl;
			break;
		default:
			std::cerr << "Unknown" << std::endl;
			break;
		}
	}
	
    _packet.clear();
}

int NetworkManager::OnRecieveAuthentication(sf::TcpSocket& _clientSocket)
{
	sf::Packet tempPacket;

	if (_clientSocket.receive(tempPacket) == sf::Socket::Status::Done)
	{
		PacketType type;
		tempPacket >> type;
		int validateAuthentication;

		switch (type)
		{
		case SV_AUTH:
		{
			tempPacket >> validateAuthentication;
			std::cout << "Mensaje recibido del servidor: " << validateAuthentication << std::endl;

			return validateAuthentication;
		}
		break;
		case SV_ROOM_CODE:
		{
			tempPacket >> validateAuthentication;
			std::cout << "Mensaje recibido del servidor: " << validateAuthentication << std::endl;

			return validateAuthentication;
		}
		break;
		default:
			break;
		}
	}
	tempPacket.clear();
	return -10;
}

std::string NetworkManager::OnRecieveRoomCode(sf::TcpSocket& _clientSocket)
{
	sf::Packet tempPacket;

	if (_clientSocket.receive(tempPacket) == sf::Socket::Status::Done)
	{
		PacketType type;
		tempPacket >> type;
		switch (type)
		{
		case SV_ROOM_CODE:
		{
			std::string roomCode;
			tempPacket >> roomCode;
			std::cout << "Mensaje recibido del servidor: " << roomCode << std::endl;

			return roomCode;
		}
		break;
		default:
			break;
		}
	}
	tempPacket.clear();
	return "";
}
