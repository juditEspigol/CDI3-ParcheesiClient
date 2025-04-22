#pragma once
#include "Button.h"

#include "ButtonTextUpdater.h"
#include "PacketType.h"

void SendData(sf::TcpSocket& _clientSocket, sf::Packet& _packet);

class ButtonPacketSender : public Button
{
private:

	PacketType packetType;
	std::vector<ButtonTextUpdater*> buttonTexts;

public:
	ButtonPacketSender(PacketType _packetType, std::vector<ButtonTextUpdater*> _buttonTexts, sf::Vector2f _position = sf::Vector2f(100, 100));
	~ButtonPacketSender();

	virtual void HandleEvent(const sf::Event& _event, sf::RenderWindow& _window, sf::TcpSocket& _socket) override;
};

