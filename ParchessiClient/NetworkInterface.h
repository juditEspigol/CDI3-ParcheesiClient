#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include "PacketType.h"

#define NETWORK_MANAGER NetworkManager::Instance()

class Client;

class NetworkManager final
{
private:

	sf::SocketSelector selector;
	sf::TcpListener listener;

	int listen = 0; // Number of clients we will listen
	int connect = 0; // Number of clients we will connect with
	
	NetworkManager() = default;
	NetworkManager(const NetworkManager&) = delete;
	NetworkManager& operator =(const NetworkManager&) = delete;

	void RegisterNewUserConnection();

public:

	inline static NetworkManager& Instance()
	{
		static NetworkManager manager;
		return manager;
	}

	void Init();
	void CheckConnections();
	void ConnectToSocket(sf::IpAddress _address);
	inline int GetListen() const { return listen; }
	inline void SetListen(const int _listen) { listen = _listen; }
	inline void SetConnect(const int _connect) { connect = _connect; }
	inline int GetConnect() const { return connect; }

	inline sf::SocketSelector GetSelector() { return selector; }
	static void SendData(sf::TcpSocket& _clientSocket, sf::Packet& _packet);
	static int OnRecieveAuthentication(sf::TcpSocket& _clientSocket);
	static std::string OnRecieveRoomCode(sf::TcpSocket& _clientSocket);
};