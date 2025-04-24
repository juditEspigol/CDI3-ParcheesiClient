#include "AuthenticateScene.h"

AuthenticateScene::AuthenticateScene()
{
	isFinished = false;
	nextScene = WAITING;

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
	Scene::HandleEvent(_event, _window, _socket);

	for (Button* button : buttons)
	{
		if (button->HasBeenPressed())
		{
			isFinished = true;
		}
	}
}
