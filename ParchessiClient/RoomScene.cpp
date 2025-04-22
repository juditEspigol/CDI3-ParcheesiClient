#include "RoomScene.h"

RoomScene::RoomScene()
{
	isFinished = false;
	nextScene = GAMEPLAY;
}

void RoomScene::OnEnter()
{
	ButtonTextUpdater* code = new ButtonTextUpdater("Insert Code...", sf::Vector2f(1280 / 2 - 135, 720 / 2 - 100));
	ButtonPacketSender* btSendCode = new ButtonPacketSender(PacketType::ROOM, std::vector<ButtonTextUpdater*>{code}, sf::Vector2f(1280 / 2 - 90, 720 / 2));

	buttons = { code, btSendCode };
}

void RoomScene::HandleEvent(const sf::Event& _event, sf::RenderWindow& _window, sf::TcpSocket& _socket)
{
	Scene::HandleEvent(_event, _window, _socket);
}
