#pragma once
#include <SFML/Network.hpp>
#include "PacketType.h"
#include <iostream>

class NetworkInterface final
{
private:
	NetworkInterface() = delete;
	NetworkInterface(const NetworkInterface&) = delete;
	NetworkInterface& operator =(const NetworkInterface&) = delete;

public:

	static void SendData(sf::TcpSocket& _clientSocket, sf::Packet& _packet);
	static int OnRecieveAuthentication(sf::TcpSocket& _clientSocket);
	static std::string OnRecieveRoomCode(sf::TcpSocket& _clientSocket);
};