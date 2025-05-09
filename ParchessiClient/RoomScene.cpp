#include "RoomScene.h"

RoomScene::RoomScene()
{
	isFinished = false;
	waitingPacket = false;
	nextScene = WAITING;

	ButtonTextUpdater* code = new ButtonTextUpdater("Insert Code...", sf::Vector2f(WIDTH / 2 - 220, HEIGHT / 2 - 20));
	ButtonPacketSender* btSendCode = new ButtonPacketSender(PacketType::JOIN_ROOM, std::vector<ButtonTextUpdater*>{code}, sf::Vector2f(WIDTH / 2 + 40, HEIGHT / 2 - 20));
	ButtonPacketSender* btCreateCode = new ButtonPacketSender(PacketType::CREATE_ROOM, std::vector<ButtonTextUpdater*>{}, sf::Vector2f(WIDTH / 2 - 90, HEIGHT / 2 + 100));

	buttons = { code, btSendCode, btCreateCode };
}

void RoomScene::OnEnter()
{
	
}

void RoomScene::HandleEvent(const sf::Event& _event, sf::RenderWindow& _window, sf::TcpSocket& _socket)
{
	if (waitingPacket && !isFinished)
	{
		std::string roomCodeRecieved = NetworkManager::OnRecieveRoomCode(_socket);
		if (roomCodeRecieved != "")
		{
			if (roomCodeRecieved == "-1")
			{
				waitingPacket = false;
				return;
			}
			CODE = roomCodeRecieved;
			isFinished = true;
			return;
		}
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
