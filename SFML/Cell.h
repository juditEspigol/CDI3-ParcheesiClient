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
	std::vector<Token*> _tokens;
	sf::RectangleShape _shape;
	sf::Vector2f _position;

public:
	Cell(int id, sf::Vector2f position);
	void Draw(sf::RenderWindow& window);

	void AddPlayer(Token* token);
	void RemovePlayer(Token* token);

	inline int GetId() { return _id; }
	inline sf::Vector2f GetPosition() { return _position;}
	std::vector<Token*> GetTokens();

};