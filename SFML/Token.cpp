#include "Token.h"


Token::Token(int playerId, int _idPos)
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
	_idPosition = _idPos;
	_shape.setOrigin(sf::Vector2f(TOKEN_RADIUS, TOKEN_RADIUS));
	_shape.setRadius(TOKEN_RADIUS);	
	_isMoving = false;

}

void Token::Draw(sf::RenderWindow& window)
{
	window.draw(_shape);
}

void Token::Move(int value)
{
	_isMoving = true;
	_idPosition += value;
}

void Token::SetPosition(sf::Vector2f newPositon)
{
	_position = newPositon;
	_shape.setPosition(newPositon);
}

void Token::EndMove()
{
	_isMoving = false;
}
