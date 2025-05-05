#pragma once
#include <SFML/Network.hpp>

enum PacketType 
{ 
    LOGIN,
    REGISTER,
    CREATE_ROOM,
    JOIN_ROOM,
    DICE_ROLL,
    END_TURN,
    SV_AUTH,
    SV_ROOM_CODE,
    SV_SOCKET
};

sf::Packet& operator>>(sf::Packet& _packet, PacketType& _type);