#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

#define TOKEN_RADIUS 15
#define SELECTION_RADIUS (TOKEN_RADIUS + 5)

class Token
{
private:
	int _playerId;
	int _exitCell;
	int _finalCell;
	bool _inBase;
	bool _isMoving;
	bool _selectable;
	bool _isLastZone;
	int _idPosition;
	sf::Vector2f _position;
	sf::CircleShape _shape;
	sf::CircleShape _selectionIndicator;

public:
	Token(int playerId, int _idPosition);
	void Draw(sf::RenderWindow& window);
	int Move(int value);
	void Dead();
	void SetPosition(sf::Vector2f newPositon, int idCell);
	void EndMove();
	void UpdateIdPosition(int value);
	void ArriveLastZone();
	inline int GetIdPosition() { return _idPosition;}

	inline void SetSelectable(bool selectable) { _selectable = selectable; }

	inline bool GetIsSelectable() { return _selectable; }
	inline bool GetIsLastZone() { return _isLastZone; }
	inline bool GetIsMoving() { return _isMoving; }
	inline bool GetIsInBase() { return _inBase; }
	inline int GetFinalCellId() { return _finalCell; }
	inline int GetPlayerId() { return _playerId;  }
	inline sf::Vector2f GetPosition() { return _position; }

};