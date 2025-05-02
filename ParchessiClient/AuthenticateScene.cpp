#include "AuthenticateScene.h"

AuthenticateScene::AuthenticateScene()
{
	isFinished = false;
	waitingPacket = false;
	nextScene = ROOM;

	// Create buttons
	ButtonTextUpdater* user = new ButtonTextUpdater("Insert User...", sf::Vector2f(1280 / 2 - 135, 720 / 2 - 100));
	ButtonTextUpdater* password = new ButtonTextUpdater("Insert Password...", sf::Vector2f(1280 / 2 - 135, 720 / 2));
	ButtonPacketSender* btLogin = new ButtonPacketSender(PacketType::LOGIN, std::vector<ButtonTextUpdater*>{user, password}, sf::Vector2f(1280 / 2 - 200, 720 / 2 + 100));
	ButtonPacketSender* btRegister = new ButtonPacketSender(PacketType::REGISTER, std::vector<ButtonTextUpdater*>{user, password}, sf::Vector2f(1280 / 2, 720 / 2 + 100));
	// Add buttons to the vector
	buttons = { user, password, btLogin, btRegister };
}

void AuthenticateScene::OnEnter()
{
}

void AuthenticateScene::HandleEvent(const sf::Event& _event, sf::RenderWindow& _window, sf::TcpSocket& _socket)
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
			case SV_AUTH:
			{
				int validateAuthentication;
				tempPacket >> validateAuthentication;
				std::cout << "Mensaje recibido del servidor: " << validateAuthentication << std::endl;

				if (validateAuthentication >= 0)
				{
					isFinished = true;
				}
				else
				{
					waitingPacket = false;
					std::cout << "Waiting: " << waitingPacket << std::endl;
				}
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
