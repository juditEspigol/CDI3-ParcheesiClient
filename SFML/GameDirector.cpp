#include "GameDirector.h"

GameDirector::GameDirector(Table& table) :
    _table(table), 
    _currentPlayer(1), 
    _diceValue(0),
    _currentState(GameState::WAITING_TURN), 
    _rng(std::random_device{}()) 
{}

void GameDirector::RollDice()
{
    if (_currentState != GameState::WAITING_TURN) return;

    _diceValue = rand() % 6 + 1;

    std::cout << "Dice Value = " << _diceValue << std::endl;

    _currentState = GameState::DICE_ROLLED;

    if (_movableTokens.empty())
    {
    }
}
