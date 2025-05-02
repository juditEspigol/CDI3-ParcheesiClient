#pragma once
#include "Scene.h"

class RoomScene : public Scene
{
private:

public:
	RoomScene();
	void OnEnter() override;

	void HandleEvent(const sf::Event& _event, sf::RenderWindow& _window, sf::TcpSocket& _socket) override;
};
