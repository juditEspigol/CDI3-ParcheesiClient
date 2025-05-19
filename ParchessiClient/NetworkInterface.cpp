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
	if (listen <= 0)
		return; 

	std::cout << "Estoy esdcuchando --> " << listen << std::endl;

	if (selector.wait())
	{
		if (selector.isReady(listener))
		{
			RegisterNewUserConnection();
			return;
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
						// window.close();
						client->GetSocket()->disconnect();
						selector.remove(*client->GetSocket());

					}
				}
			}
		}
	}
}

void NetworkManager::ConnectToSocket(sf::IpAddress _address)
{
	if (connect <= 0) // detect if the client have to connect with someone
		return;

	Client* newClient = new Client(0, new sf::TcpSocket());

	std::cout << "Trying to connect with... " << _address.toString() << "..." << std::endl;

	if (newClient->GetSocket()->connect(_address, LISTENER_PORT) != sf::Socket::Status::Done)
	{
		std::cerr << "Error connecting to client: " << _address.toString() << std::endl;
		delete newClient;
		return;
	}

	CLIENT_MANAGER.AddClient(newClient);
	newClient->GetSocket()->setBlocking(false);
	newClient->SetID(CLIENT_MANAGER.GetSizeClients());
	std::cout << "Connect to other client --> (" << newClient->GetID() << ") --> " << newClient->GetSocket()->getRemoteAddress().value() << std::endl;

	connect--;
	return;
}

void NetworkManager::RegisterNewUserConnection()
{	
	Client* newClient = new Client(0, new sf::TcpSocket());

	if (listener.accept(*newClient->GetSocket()) == sf::Socket::Status::Done) // Añadir nuevo cliente HANDSHAKE
	{
		newClient->GetSocket()->setBlocking(false); // Desbloqueamos el socket
		selector.add(*newClient->GetSocket());

		std::cout << "Nueva conexion establecida --> " << newClient->GetIP() << "..." << std::endl;

		CLIENT_MANAGER.AddClient(newClient);

		newClient->SetID(CLIENT_MANAGER.GetSizeClients() + 1);

		std::cout << "Connect to other client --> (" << newClient->GetID() << ") --> " << newClient->GetSocket()->getRemoteAddress().value() << std::endl;
		
		listen--;
		return;
	}
	else
	{
		std::cout << "Intento de connexion no valido" << std::endl;
		delete newClient;
	}
}

void NetworkManager::SendData(sf::TcpSocket& _clientSocket, sf::Packet& _packet)
{
	if (_clientSocket.send(_packet) != sf::Socket::Status::Done)
	{
		std::cerr << "Error al enviar el paquete al servidor" << std::endl;
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
