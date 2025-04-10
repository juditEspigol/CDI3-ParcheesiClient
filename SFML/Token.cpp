#include "Token.h"

Token::Token(int playerId)
{
	switch (playerId)
	{
	case 1:
		_shape.setFillColor(sf::Color::Blue);
		break;

	case 2:
		_shape.setFillColor(sf::Color::Red);
		break;

	case 3:
		_shape.setFillColor(sf::Color::Black);
		break;

	case 4:
		_shape.setFillColor(sf::Color::Yellow);
		break;
	default:
		break;
	}

	_shape.setRadius(4);
	
	//_shape.setPosition(TABLE.GetCell(10)->GetPosition());	
}

void Token::Draw(sf::RenderWindow& window)
{
	window.draw(_shape);
	window.display();
}

void Token::Move(int value)
{
}
