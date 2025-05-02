#pragma once
#include "Scene.h"

class AuthenticateScene : public Scene
{
private:

public:
	AuthenticateScene();
	void OnEnter() override;

	void HandleEvent(const sf::Event& _event, sf::RenderWindow& _window, sf::TcpSocket& _socket) override;
};

