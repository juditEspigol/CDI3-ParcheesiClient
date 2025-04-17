#include "Cell.h"


Cell::Cell(int id, sf::Vector2f position)
{
	_id = id;
	_position = position;	
	sf::Vector2f newSize = sf::Vector2f(CELL_WIDTH, CELL_HEIGTH);
	_shape.setFillColor(sf::Color::Magenta);
	_shape.setSize(newSize);
	_shape.setPosition(position);
}

void Cell::Draw(sf::RenderWindow& window)
{
	window.draw(_shape);
	window.display();
}

void Cell::AddToken(Token* token)
{
	for (Token* a : _tokens)
	{
		if (a == token)
		{
			return;
		}
	}
	_tokens.push_back(token);
}

void Cell::RemoveToken(Token* token)
{
	for (size_t i = 0; i < _tokens.size(); ++i)
	{
		if (_tokens[i] == token)
		{
			_tokens.erase(_tokens.begin() + i);
			break;
		}
	}
}

