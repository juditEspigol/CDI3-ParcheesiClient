#include "Token.h"


Token::Token(int playerId, int _idPos, int tokenID)
{
	switch (playerId)
	{
	case 1:
		_shape.setFillColor(sf::Color::Blue);
		_exitCell = 22;
		_finalCell = 17;
		break;
	case 2:
		_shape.setFillColor(sf::Color::Red);
		_exitCell = 39;
		_finalCell = 34;
		break;
	case 3:
		_shape.setFillColor(sf::Color::Green);
		_exitCell = 56;
		_finalCell = 51;
		break;
	case 4:
		_shape.setFillColor(sf::Color::Yellow);
		_exitCell = 5;
		_finalCell = 68;
		break;
	default:
		break;
	}

	_idPosition = _idPos;
	_isMoving = false;
	_playerId = playerId;
	_selectable = false;
	_tokenId = tokenID;


	_shape.setOrigin(sf::Vector2f(TOKEN_RADIUS, TOKEN_RADIUS));
	_shape.setRadius(TOKEN_RADIUS);
	_shape.setOutlineThickness(TOKEN_OUTLINE_THICKNESS);
	_shape.setOutlineColor(TOKEN_OUTLINE_COLOR);

	_selectionIndicator.setOrigin(sf::Vector2f(SELECTION_RADIUS, SELECTION_RADIUS));
	_selectionIndicator.setRadius(SELECTION_RADIUS);
	_selectionIndicator.setFillColor(sf::Color(128, 0, 128));
	_inBase = true;
	_isLastZone = false;
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
	
	if (_inBase)
	{
		_inBase = false;
		return _exitCell;
	}

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

void Token::ArriveLastZone()
{
	_isLastZone = true;
}
