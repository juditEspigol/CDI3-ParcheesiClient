#pragma once

#include <memory>
#include <random>

#include "ButtonPacketSender.h"
#include "IGameStateProvider.h"

#define PLAYER_INDICATOR_SIZE 50
#define DICE_INDICATOR_SIZE 45

class Dice : public ButtonPacketSender
{
public:
	Dice(IGameStateProvider* provider);
	void OnLeftClick(const sf::Event::MouseButtonPressed* _mousePressed, sf::TcpSocket& _socket) override;
	void UnselectButton() override;

	void RollDice();
	void ForceDiceValue(int value);
	sf::Text GetDiceText();
	sf::RectangleShape GetTurnIndicator(int currentPlayer, float width, float height);

private:
	int _diceValue;
	IGameStateProvider* stateProvider;
	std::mt19937 _rng;
	sf::Font _font;
	sf::RectangleShape _turnIndicator;

public:
	inline int GetDiceValue() { return _diceValue; }
	inline void SetDiceValue(int diceValue) { _diceValue = diceValue; }
};

