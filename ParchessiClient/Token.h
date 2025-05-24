#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

#include "ButtonPacketSender.h"
#include "IGameStateProvider.h"
#include "ClientManager.h"

#define TOKEN_RADIUS 15
#define SELECTION_RADIUS (TOKEN_RADIUS + 5)
#define TOKEN_OUTLINE_THICKNESS 2.0f
#define TOKEN_OUTLINE_COLOR sf::Color::Black

class Token : public ButtonPacketSender
{
private:
	int _playerId;
	int _exitCell;
	int _finalCell;

	bool _inBase;
	bool _isMoving;
	bool _isLastZone;

	int _idPosition;
	int _tokenId;

	sf::Vector2f _position;
	sf::CircleShape _shape;
	sf::CircleShape _selectionIndicator;
	IGameStateProvider* _stateProvider;
public:
	Token(int playerId, int _idPosition, int tokenID);
	void OnLeftClick(const sf::Event::MouseButtonPressed* _mousePressed, sf::TcpSocket& _socket) override;
	
	void Draw(sf::RenderWindow& window);
	int Move(int value);
	void SetPosition(sf::Vector2f newPositon, int idCell);
	void EndMove();
	void UpdateIdPosition(int value);
	void ArriveLastZone();

	inline void SetSelectable(bool selectable) { selected = selectable; }
	inline void SetIsInBase(bool inBase) { _inBase = inBase; }

	inline int GetIdPosition() { return _idPosition; }
	inline bool GetIsSelectable() { return selected; }
	inline bool GetIsLastZone() { return _isLastZone; }
	inline bool GetIsMoving() { return _isMoving; }
	inline bool GetIsInBase() { return _inBase; }
	inline int GetFinalCellId() { return _finalCell; }
	inline int GetPlayerId() { return _playerId;  }
	inline int GetTokenId() { return _tokenId; }
	inline sf::Vector2f GetPosition() { return _position; }

};