#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

#define TOKEN_RADIUS 15

class Token
{
private:
	int _playerId;
	bool _inBase;
	bool _isMoving;
	int _idPosition;
	sf::Vector2f _position;
	sf::CircleShape _shape;

public:
	Token(int playerId, int _idPosition);
	void Draw(sf::RenderWindow& window);
	void Move(int value);
	void SetPosition(sf::Vector2f newPositon);
	void EndMove();
	inline int GetIdPosition() { return _idPosition;}
	inline bool GetIsMoving() { return _isMoving; }
	inline sf::Vector2f GetPosition() { return _position; }
	inline int GetPlayerId() { return _playerId; }

};