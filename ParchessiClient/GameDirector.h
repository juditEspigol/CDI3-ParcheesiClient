#pragma once

#include <vector>
#include <memory>
#include <random>

#include "Table.h"
#include "Token.h"
#include "Dice.h"

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
    GameDirector(Table& table, Dice* dice);

private:
    int _currentPlayer;
    Table& _table;
    Dice* _dice;

    GameState _currentState;
    Token* _selectedToken;

    std::vector<Token*> _movableTokens;

    bool CanTokenMove(Token& token);
    bool IsTokenFromCurrentPlayer(Token& token);

    void EndTurn();

public:
    void StartGame();
    void StartPlayerTurn(int playerId);
    void SelectToken(sf::Vector2i mousePos);
    void MoveSelectedToken();
    void CalculateMovableTokens();

    inline int GetCurrentPlayer() { return _currentPlayer; };
    inline std::vector<Token*>& GetMovableTokens() { return _movableTokens; }
    inline GameState GetCurrentState() { return _currentState; }
};