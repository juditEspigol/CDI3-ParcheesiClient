#include "RoomScene.h"

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
		//OnRecievePacket(_socket);
		sf::Packet tempPacket;

		if (_socket.receive(tempPacket) == sf::Socket::Status::Done)
		{
			PacketType type;
			tempPacket >> type;
			switch (type)
			{
			case SV_ROOM_CODE:
			{
				std::string roomCode;
				tempPacket >> roomCode;
				std::cout << "Mensaje recibido del servidor: " << roomCode << std::endl;

				isFinished = true;
				waitingPacket = false;
				return;
			}
			break;
			default:
				break;
			}
		}
		else
		{
			std::cerr << "Error al recibir el mensaje del servidor" << std::endl;
		}
		tempPacket.clear();
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
