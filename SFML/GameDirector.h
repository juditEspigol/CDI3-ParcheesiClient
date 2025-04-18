#pragma once

#include <vector>
#include <memory>
#include <random>

#include "Table.h"
#include "Token.h"

class GameDirector
{
public:
    enum class GameState 
    {
        WAITING_TURN,
        DICE_ROLLED,
        PIECE_SELECTED,
        TURN_COMPLETE
    };
    GameDirector(Table& table);


private:
    Table& _table;
    int _currentPlayer;
    int _diceValue;
    GameState _currentState;
    std::vector<int> _movableTokens;
    int _selectedTokenId;
    std::mt19937 _rng;

    void CalculateMovableTokens();

    bool CanTokenMove(Token& token);
    bool IsTokenFromCurrentPlayer(Token& token);

    void EndTurn();

public:
    void StartGame();
    void StartPlayerTurn(int playerId);
    void RollDice();
    void SelectToken(int tokenId);
    void MoveSelectedToken();

    inline int GetCurrentPlayer() { return _currentPlayer; };
    inline int GetDiceValue() { return _diceValue; };
    inline std::vector<int>& GetMovableTokens() { return _movableTokens; }
    inline GameState GetCurrentState() { return _currentState; }
};