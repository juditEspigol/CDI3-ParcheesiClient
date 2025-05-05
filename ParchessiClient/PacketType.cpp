#include "PacketType.h"

sf::Packet& operator>>(sf::Packet& _packet, PacketType& _type)
{
 
    int temp;
    _packet >> temp;
    _type = static_cast<PacketType>(temp);

    return _packet;

}
