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

    void StartGame();
    void StartPlayerTurn(int playerId);
    void RollDice();
    void SelectToken(int tokenId);
    void MoveSelectedToken();

    int GetCurrentPlayer() const;
    int GetDiceValue() const;
    GameState GetCurrentState() const;
    const std::vector<int>& GetMovableTokens() const;

private:
    Table& _table;
    int _currentPlayer;
    int _diceValue;
    GameState _currentState;
    std::vector<int> _movableTokens;
    int _selectedTokenId;
    std::mt19937 _rng;

    void CalculateMovableTokens();
    bool CanTokenMove(const Token& token) const;
    bool IsTokenFromCurrentPlayer(const Token& token) const;
    void EndTurn();
};