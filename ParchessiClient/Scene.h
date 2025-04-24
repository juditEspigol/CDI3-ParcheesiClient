#pragma once
#include "ButtonPacketSender.h"
#include "ButtonTextUpdater.h"

#define AUTHENTICATION "AuthenticateScene"
#define ROOM "RoomScene"
#define WAITING "WaitingScene"
#define GAMEPLAY "Gameplay"

class Scene
{
protected:

	bool isFinished;
	std::string nextScene;
	std::vector<Button*> buttons; 

	void GeneralEvents(const sf::Event& _event, sf::RenderWindow& _window, sf::TcpSocket& _socket);

public:
	Scene() = default;
	virtual void OnEnter() = 0; 
	virtual std::string OnExit(); 

	virtual void HandleEvent(const sf::Event& _event, sf::RenderWindow& _window, sf::TcpSocket& _socket);
	virtual void Render(sf::RenderWindow& _window);

	inline bool GetIsFinished() const { return isFinished; }
};

