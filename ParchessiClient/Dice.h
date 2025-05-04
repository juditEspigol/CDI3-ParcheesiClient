#pragma once

#include <memory>
#include <random>

#include "ButtonPacketSender.h"

#define PLAYER_INDICATOR_SIZE 50
#define DICE_INDICATOR_SIZE 45

class Dice : public ButtonPacketSender
{
public:
	Dice();
	void OnLeftClick(const sf::Event::MouseButtonPressed* _mousePressed, sf::TcpSocket& _socket) override;

	void RollDice();
	void ForceDiceValue(int value); // Cheats to force dice to be a specific value
	sf::Text GetDiceText();
	sf::RectangleShape GetTurnIndicator(int currentPlayer, float width, float height);

private:
	int _diceValue;
	std::mt19937 _rng;
	sf::Font _font;
	sf::RectangleShape _turnIndicator;

public:
	inline int GetDiceValue() { return _diceValue; }

};

