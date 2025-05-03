#include "NetworkInterface.h"

void NetworkInterface::SendData(sf::TcpSocket& _clientSocket, sf::Packet& _packet)
{
	if (_clientSocket.send(_packet) != sf::Socket::Status::Done)
	{
		std::cerr << "Error al enviar el paquete al servidor" << std::endl;
	}
	else
	{
		std::cout << "Mensaje enviado" << std::endl;
	}
	_packet.clear();
}

int NetworkInterface::OnRecieveAuthentication(sf::TcpSocket& _clientSocket)
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
	else
	{
		std::cout << "Mensaje enviado" << std::endl;
	}
	tempPacket.clear();
	return -10;
}

std::string NetworkInterface::OnRecieveRoomCode(sf::TcpSocket& _clientSocket)
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
	else
	{
		std::cout << "Mensaje enviado" << std::endl;
	}
	tempPacket.clear();
	return "";
}
