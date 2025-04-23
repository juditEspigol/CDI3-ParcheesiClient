#pragma once

#include <vector>
#include <memory>
#include <random>

#include "Table.h"
#include "Token.h"

#define PLAYER_INDICATOR_SIZE 50
#define DICE_INDICATOR_SIZE 45

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
    int _currentPlayer;
    int _diceValue;
    Table& _table;
    GameState _currentState;
    Token* _selectedToken;

    std::vector<Token*> _movableTokens;
    std::mt19937 _rng;

    sf::RectangleShape _turnIndicator;
    sf::Font _font;

    void CalculateMovableTokens();

    bool CanTokenMove(Token& token);
    bool IsTokenFromCurrentPlayer(Token& token);

    void EndTurn();

public:
    void StartGame();
    void StartPlayerTurn(int playerId);
    void RollDice();
    void ForceDiceValue(int value); // Cheats to force dice to be a specific value
    void SelectToken(sf::Vector2i mousePos);
    void MoveSelectedToken();

    sf::Text GetDiceText();
    sf::RectangleShape GetTurnIndicator(float width, float height);
    inline int GetCurrentPlayer() { return _currentPlayer; };
    inline int GetDiceValue() { return _diceValue; };
    inline std::vector<Token*>& GetMovableTokens() { return _movableTokens; }
    inline GameState GetCurrentState() { return _currentState; }
};