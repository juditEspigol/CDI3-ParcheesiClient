#pragma once

#include "Token.h"

class Player
{
private:
	int _id;
	std::vector<Token*> _tokens;
	bool _isPlayerTurn;

public:
	Player(int id);
	void ThrowDice();
	void StartTurn();

};

