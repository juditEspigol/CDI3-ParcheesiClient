#include "Cell.h"


Cell::Cell(int id, sf::Vector2f position)
{
	_id = id;
	_position = position;
	
	sf::Vector2f newSize = sf::Vector2f(CELL_WIDTH, CELL_HEIGTH);
	_shape.setSize(newSize);
	_shape.setFillColor(sf::Color::White);
	_shape.setPosition(position);
}

void Cell::Draw(sf::RenderWindow& window)
{
	window.draw(_shape);
	window.display();
}

void Cell::AddPlayer(Token* token)
{

}

void Cell::RemovePlayer(Token* token)
{

}
