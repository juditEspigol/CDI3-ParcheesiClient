#pragma once
#include <iostream>
#include <SFML/Network.hpp>

class Client;

bool operator==(Client client, Client client2);
sf::IpAddress StringToIpAddress(const std::string& ipAdress);
sf::Packet& operator>>(sf::Packet& _packet, std::pair<sf::IpAddress, unsigned short>& _address);

class Client
{
private:
    unsigned int id;
    sf::TcpSocket* socket;

public:
    Client(const unsigned int _id, sf::TcpSocket* _socket);

    inline void SetID(const unsigned int _id) { id = _id; }

    inline sf::TcpSocket* GetSocket() const { return socket; }
    inline sf::IpAddress GetIP() const { return socket->getRemoteAddress().value(); }
    inline unsigned int GetID() const { return id; }
};