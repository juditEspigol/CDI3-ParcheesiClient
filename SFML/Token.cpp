#include "Token.h"

Token::Token(int playerId)
{

	_idPosition = 12;

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
	_shape.setOrigin({10,10});
	_shape.setRadius(10);
	
}

void Token::Draw(sf::RenderWindow& window)
{
	window.draw(_shape);
}

void Token::Move(int value)
{
	_idPosition += value;
}

void Token::SetPosition(sf::Vector2f newPositon)
{
	_shape.setPosition(newPositon);
}
