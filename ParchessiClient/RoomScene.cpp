#include "RoomScene.h"

RoomScene::RoomScene()
{
	isFinished = false;
	nextScene = WAITING;

	ButtonTextUpdater* code = new ButtonTextUpdater("Insert Code...", sf::Vector2f(1280 / 2 - 135, 720 / 2 - 100));
	ButtonPacketSender* btSendCode = new ButtonPacketSender(PacketType::JOIN_ROOM, std::vector<ButtonTextUpdater*>{code}, sf::Vector2f(1280 / 2 - 90, 720 / 2));
	ButtonPacketSender* btCreateCode = new ButtonPacketSender(PacketType::CREATE_ROOM, std::vector<ButtonTextUpdater*>{}, sf::Vector2f(1280 / 2 - 90, 720 / 2 + 100));

	buttons = { code, btSendCode, btCreateCode };
}

void RoomScene::OnEnter()
{
	
}

void RoomScene::HandleEvent(const sf::Event& _event, sf::RenderWindow& _window, sf::TcpSocket& _socket)
{
	Scene::HandleEvent(_event, _window, _socket);
}
