#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

class Token
{
private:
	int _playerId;
	int _currentCell;
	bool _inBase;
	sf::CircleShape _cell;

public:
	Token(int playerId);
	void Draw(sf::RenderWindow& window);
	void Move(int value);
};