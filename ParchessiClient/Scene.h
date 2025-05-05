#pragma once
#include "ButtonPacketSender.h"
#include "ButtonTextUpdater.h"
#include "GlobalValues.h"

#define AUTHENTICATION "AuthenticateScene"
#define ROOM "RoomScene"
#define WAITING "WaitingScene"
#define GAMEPLAY "GameplayScene"

class Scene
{
protected:

	bool isFinished;
	bool waitingPacket;
	std::string nextScene;

	std::vector<Button*> buttons; 

public:
	Scene() = default;
	virtual void OnEnter() = 0; 
	virtual std::string OnExit(); 

	inline bool GetIsFinished() const { return isFinished; }
	inline void SetIsFinished(const bool _isFinished) { isFinished = _isFinished; }
	inline std::vector<Button*> GetButtons() const { return buttons; }

	virtual void HandleEvent(const sf::Event& _event, sf::RenderWindow& _window, sf::TcpSocket& _socket);
	virtual void Update(float _dt);
	virtual void Render(sf::RenderWindow& _window);

};

