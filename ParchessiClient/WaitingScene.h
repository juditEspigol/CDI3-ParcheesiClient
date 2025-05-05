#pragma once
#include "Scene.h"

class WaitingScene : public Scene
{
private:

	std::string code;
	std::vector<TextFill*> texts; 

public:
	WaitingScene();
	void OnEnter() override;
	void Render(sf::RenderWindow& _window) override;

	void HandleEvent(const sf::Event& _event, sf::RenderWindow& _window, sf::TcpSocket& _socket) override;
};

