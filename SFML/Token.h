#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

#define TOKEN_RADIUS 15
#define SELECTION_RADIUS (TOKEN_RADIUS + 5)

class Token
{
private:
	int _playerId;
	bool _inBase;
	bool _isMoving;
	bool _selectable;
	int _idPosition;
	sf::Vector2f _position;
	sf::CircleShape _shape;
	sf::CircleShape _selectionIndicator;

public:
	Token(int playerId, int _idPosition);
	void Draw(sf::RenderWindow& window);
	int Move(int value);
	void Dead();
	void SetPosition(sf::Vector2f newPositon, int idCell);
	void EndMove();
	void UpdateIdPosition(int value);
	inline int GetIdPosition() { return _idPosition;}

	inline void SetSelectable(bool selectable) { _selectable = selectable; }

	inline bool GetIsSelectable() { return _selectable; }
	inline bool GetIsMoving() { return _isMoving; }
	inline int GetPlayerId() { return _playerId;  }
	inline sf::Vector2f GetPosition() { return _position; }

};