#pragma once
#include <SFML/Network.hpp>

enum PacketType 
{ 
    LOGIN,
    REGISTER,
    CREATE_ROOM,
    JOIN_ROOM,
    SV_AUTH,
    SV_ROOM_CODE,
    SV_SOCKET,
    SV_CONNECT_DATA,
    DICE_ROLL,
    END_TURN,
    MOVE_TOKEN
};

sf::Packet& operator>>(sf::Packet& _packet, PacketType& _type);