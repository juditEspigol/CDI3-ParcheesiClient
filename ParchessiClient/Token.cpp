#include "Token.h"


Token::Token(int playerId, int _idPos, int tokenID) :
ButtonPacketSender(MOVE_TOKEN, {}, sf::Vector2f(0, 0))
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
	selected = false;
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

void Token::OnLeftClick(const sf::Event::MouseButtonPressed* _mousePressed, sf::TcpSocket& _socket)
{
	if (!_stateProvider->IsTokenMoveAllowed())
		return;

	if (_shape.getGlobalBounds().contains(sf::Vector2f(_mousePressed->position)))
	{
		sf::Packet packet;

		selected = true;
		std::cout << "Token Selected" << std::endl;

		int diceValue = dynamic_cast<IGameStateProvider*>(_stateProvider)->GetDiceValue();
		int newPosition = Move(diceValue);
		std::cout << "Sended Token" << std::endl;

		for (Client* client : CLIENT_MANAGER.GetClients())
		{
			std::cout << "Client Id: " << client->GetID() << ". Sended Packet Token " << _tokenId << std::endl;
			packet << packetType << _tokenId << newPosition;
			NETWORK_MANAGER.SendData(*client->GetSocket(), packet);
		}
	}
}

void Token::Draw(sf::RenderWindow& window)
{
	if (selected)
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