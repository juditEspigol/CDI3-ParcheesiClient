#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Token.h"

#define CELL_WIDTH 32
#define CELL_HEIGTH 32

class Cell
{
private:
	int _id;
	bool _finalCell;
	bool _isHorizontal;
	std::vector<Token*> _tokens;
	sf::RectangleShape _shape;
	sf::Vector2f _position;

public:
	Cell(int id, sf::Vector2f position, bool _isHoriz);

	void AddToken(Token* token);
	void RemoveToken(Token* token);

	inline int GetId() { return _id; }
	inline sf::Vector2f GetPosition() { return _position;}
	inline bool GetHoritzontal() { return _isHorizontal; }
	std::vector<Token*> GetTokens();

};