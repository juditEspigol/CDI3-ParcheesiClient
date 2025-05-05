#include "Client.h"

bool operator==(Client client, Client client2)
{
    if (client.GetID() != client2.GetID() ||
        client.GetIP() != client2.GetIP() ||
        client.GetSocket() != client2.GetSocket())
    {
        return false;
    }
    return true;
}

sf::IpAddress StringToIpAddress(const std::string& ipAdress)
{
    unsigned short a, b, c, d;

    if (sscanf_s(ipAdress.c_str(), R"(%u.%u.%u.%u)", &a, &b, &c, &d) != 4)
        return sf::IpAddress::Any;

    return sf::IpAddress(a, b, c, d);
}

sf::Packet& operator>>(sf::Packet& _packet, std::pair<sf::IpAddress, unsigned short>& _address)
{
    std::string ipAdress;

    if (!(_packet >> ipAdress >> _address.second))
    {
        std::cerr << "Error on reading IP Adress / Port" << std::endl;
        return _packet;
    }

    _address.first = StringToIpAddress(ipAdress);

    return _packet;
}

Client::Client(const unsigned int _id, sf::TcpSocket* _socket) : socket(_socket) {}
