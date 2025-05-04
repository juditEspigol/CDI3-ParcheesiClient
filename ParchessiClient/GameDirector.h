#pragma once

#include <vector>
#include <memory>
#include <random>

#include "IGameStateProvider.h"
#include "Table.h"
#include "Token.h"
#include "Dice.h"
#include "EndTurnButton.h"

class GameDirector : public IGameStateProvider
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
    int _currentPlayer;
    Table& _table;
    Dice* _dice;
    EndTurnButton* _endTurn;

    GameState _currentState;
    Token* _selectedToken;

    std::vector<Token*> _movableTokens;

    bool CanTokenMove(Token& token);
    bool IsTokenFromCurrentPlayer(Token& token);

public:
    void StartGame();
    void StartPlayerTurn(int playerId);
    void SelectToken(sf::Vector2i mousePos);
    void MoveSelectedToken();
    void CalculateMovableTokens();
    void EndTurn();

    bool IsDiceRollAllowed() const override;
    bool IsEndTurnAllowed() const override;

    inline void SetEndTurn(EndTurnButton* endTurn) { _endTurn = endTurn; }
    inline void SetDice(Dice* dice) { _dice = dice; }
    
    inline int GetCurrentPlayer() { return _currentPlayer; };
    inline std::vector<Token*>& GetMovableTokens() { return _movableTokens; }
    inline GameState GetCurrentState() { return _currentState; }
};