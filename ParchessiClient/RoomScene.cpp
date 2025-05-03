#include "RoomScene.h"
#include "GlobalValues.h"

RoomScene::RoomScene()
{
	isFinished = false;
	waitingPacket = false;
	nextScene = WAITING;

	ButtonTextUpdater* code = new ButtonTextUpdater("Insert Code...", sf::Vector2f(1280 / 2 - 220, 720 / 2 - 20));
	ButtonPacketSender* btSendCode = new ButtonPacketSender(PacketType::JOIN_ROOM, std::vector<ButtonTextUpdater*>{code}, sf::Vector2f(1280 / 2 + 40, 720 / 2 - 20));
	ButtonPacketSender* btCreateCode = new ButtonPacketSender(PacketType::CREATE_ROOM, std::vector<ButtonTextUpdater*>{}, sf::Vector2f(1280 / 2 - 90, 720 / 2 + 100));

	buttons = { code, btSendCode, btCreateCode };
}

void RoomScene::OnEnter()
{
	
}


void RoomScene::HandleEvent(const sf::Event& _event, sf::RenderWindow& _window, sf::TcpSocket& _socket)
{
	if (waitingPacket && !isFinished)
	{
		// 1. Pressed create code room
		std::string roomCodeRecieved = NetworkInterface::OnRecieveRoomCode(_socket);
		if (roomCodeRecieved != "")
		{
			if (roomCodeRecieved == "-1")
			{
				waitingPacket = false;
				return;
			}
			CODE = roomCodeRecieved;
			isFinished = true;
		}

		// 2. Pressed join room
		int validateAuthentication = NetworkInterface::OnRecieveAuthentication(_socket);
		if (validateAuthentication >= 0)
		{
			isFinished = true;
		}
		else if (validateAuthentication == -1) // No es valido
		{
			waitingPacket = false;
		}
		return;
	}

	Scene::HandleEvent(_event, _window, _socket);

	for (Button* button : buttons)
	{
		// I havce pressed a LOG IN / REGISTER button with correct information
		if (button->HasBeenPressed()) // has send packet
		{
			waitingPacket = true;
			button->SetBeenPressed(false);
		}
	}
}
