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
		_shape.setFillColor(sf::Color::Green);
		break;
	case 4:
		_shape.setFillColor(sf::Color::Yellow);
		break;
	default:
		break;
	}

	_idPosition = _idPos;
	_isMoving = false;
	_playerId = playerId;
	_selectable = false;

	_shape.setOrigin(sf::Vector2f(TOKEN_RADIUS, TOKEN_RADIUS));
	_shape.setRadius(TOKEN_RADIUS);

	std::cout << "Current Token Player: "  << _playerId << std::endl;

	_selectionIndicator.setOrigin(sf::Vector2f(SELECTION_RADIUS, SELECTION_RADIUS));
	_selectionIndicator.setRadius(SELECTION_RADIUS);
	_selectionIndicator.setFillColor(sf::Color(128, 0, 128));
}

void Token::Draw(sf::RenderWindow& window)
{
	if (_selectable)
	{
		_selectionIndicator.setPosition(_position);
		window.draw(_selectionIndicator);
	}

	window.draw(_shape);
}

int Token::Move(int value)
{
	_isMoving = true;

	return _idPosition + value;
}

void Token::SetPosition(sf::Vector2f newPositon, int idCell)
{
	_idPosition = idCell;
	_position = newPositon;
	_shape.setPosition(newPositon);
}

void Token::EndMove()
{
	_isMoving = false;
}

void Token::UpdateIdPosition(int value)
{
	_idPosition = value;
}
