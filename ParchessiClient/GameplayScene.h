#pragma once
#include "Scene.h"
#include "GameDirector.h"

class GameplayScene : public Scene
{
private:
	sf::Sprite* tableSprite;
	Table* table;
	GameDirector* gameDirector;

public:
	GameplayScene();
	~GameplayScene();
	void OnEnter() override;

	void HandleEvent(const sf::Event& _event, sf::RenderWindow& _window, sf::TcpSocket& _socket) override;
	void Render(sf::RenderWindow& _window) override;
	void Update(float _dt) override;

};