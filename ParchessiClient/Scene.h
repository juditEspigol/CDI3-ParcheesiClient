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

public:
	Scene() = default;
	virtual void OnEnter() = 0; 
	virtual std::string OnExit(); 

	inline bool GetIsFinished() const { return isFinished; }
	inline std::vector<Button*> GetButtons() const { return buttons; }

	virtual void HandleEvent(const sf::Event& _event, sf::RenderWindow& _window, sf::TcpSocket& _socket);
	virtual void Update(float _dt);
	virtual void Render(sf::RenderWindow& _window);

};

