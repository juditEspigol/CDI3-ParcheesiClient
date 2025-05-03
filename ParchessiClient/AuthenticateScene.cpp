#include "AuthenticateScene.h"
#include "NetworkInterface.h"

AuthenticateScene::AuthenticateScene()
{
	isFinished = false;
	waitingPacket = false;
	nextScene = ROOM;

	// Create buttons
	ButtonTextUpdater* user = new ButtonTextUpdater("Insert User...", sf::Vector2f(WIDTH / 2 - 135, HEIGHT / 2 - 100));
	ButtonTextUpdater* password = new ButtonTextUpdater("Insert Password...", sf::Vector2f(WIDTH / 2 - 135, HEIGHT / 2));
	ButtonPacketSender* btLogin = new ButtonPacketSender(PacketType::LOGIN, std::vector<ButtonTextUpdater*>{user, password}, sf::Vector2f(WIDTH / 2 - 200, HEIGHT / 2 + 100));
	ButtonPacketSender* btRegister = new ButtonPacketSender(PacketType::REGISTER, std::vector<ButtonTextUpdater*>{user, password}, sf::Vector2f(WIDTH / 2, HEIGHT / 2 + 100));
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
		int validateAuthentication = NetworkInterface::OnRecieveAuthentication(_socket);
		if (validateAuthentication >= 0)
		{
			isFinished = true;
		}
		else if(validateAuthentication == -1) // No valid
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
