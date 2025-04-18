#include "GameDirector.h"

GameDirector::GameDirector(Table& table) :
    _table(table), 
    _currentPlayer(1), 
    _diceValue(0),
    _currentState(GameState::WAITING_TURN), 
    _rng(std::random_device{}()) 
{}

void GameDirector::StartGame()
{
    _currentPlayer = 1;
    StartPlayerTurn(_currentPlayer);
}

void GameDirector::StartPlayerTurn(int playerId)
{
    _currentPlayer = playerId;
    _currentState = GameState::WAITING_TURN;
    _movableTokens.clear();
    _selectedTokenId = -1;
}

void GameDirector::RollDice()
{
    if (_currentState != GameState::WAITING_TURN) return;

    std::uniform_int_distribution<int> dist(1, 6);
    _diceValue = dist(_rng);

    std::cout << "Dice Value = " << _diceValue << std::endl;

    CalculateMovableTokens();
    _currentState = GameState::DICE_ROLLED;

    if (_movableTokens.empty())
    {
        EndTurn();
    }
}

void GameDirector::SelectToken(int tokenId)
{
}

void GameDirector::MoveSelectedToken()
{
}

void GameDirector::CalculateMovableTokens()
{
    // Poner lo de MLG aqui
}

bool GameDirector::CanTokenMove(Token& token)
{
    // Comrpobar si el token es movible
    return true;
}

bool GameDirector::IsTokenFromCurrentPlayer(Token& token)
{
    if (_currentPlayer == token.GetPlayerId())
    {
        return true;
    }
    return false;
}

void GameDirector::EndTurn()
{
    _currentPlayer = _currentPlayer % 4 + 1;
    StartPlayerTurn(_currentPlayer);
}
