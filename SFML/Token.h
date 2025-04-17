#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

#define TOKEN_RADIUS 15

class Token
{
private:
	int _playerId;
	int _currentCell;
	bool _inBase;
	int _idPosition;
	sf::Vector2f _position;
	sf::CircleShape _shape;

public:
	Token(int playerId);
	void Draw(sf::RenderWindow& window);
	void Move(int value);
	void SetPosition(sf::Vector2f newPositon);
	inline int GetIdPosition() { return _idPosition;}
	inline sf::Vector2f GetPosition() { return _position; }
};