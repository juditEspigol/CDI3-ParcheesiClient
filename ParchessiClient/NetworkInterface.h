#pragma once
#include <SFML/Network.hpp>
#include "PacketType.h"
#include <iostream>

#define NETWORK_MANAGER NetworkManager::Instance()

class Client;

class NetworkManager final
{
private:

	sf::SocketSelector selector;
	sf::TcpListener listener;
	
	NetworkManager() = default;
	NetworkManager(const NetworkManager&) = delete;
	NetworkManager& operator =(const NetworkManager&) = delete;

public:

	inline static NetworkManager& Instance()
	{
		static NetworkManager manager;
		return manager;
	}

	void Init();
	void CheckConnections();
	void RegisterNewUserConnection(Client* _newClient);
	static void SendData(sf::TcpSocket& _clientSocket, sf::Packet& _packet);
	static int OnRecieveAuthentication(sf::TcpSocket& _clientSocket);
	static std::string OnRecieveRoomCode(sf::TcpSocket& _clientSocket);
};