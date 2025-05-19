#pragma once

#include <vector>
#include <memory>
#include <random>

#include "IGameStateProvider.h"
#include "Table.h"
#include "Token.h"
#include "Dice.h"
#include "EndTurnButton.h"


#define PLAYER_INDICATOR_SIZE 50
#define DICE_INDICATOR_SIZE 45

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
    GameState _currentState;
    Token* _selectedToken;
    Dice* _dice;
	EndTurnButton* _endTurnButton;

    int _newTokenPosition;

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
    int GetCurrentPlayer() const override;

	inline void SetEndTurn(EndTurnButton* endTurnButton) { _endTurnButton = endTurnButton; }
	inline void SetDice(Dice* dice) { _dice = dice; }
    inline void SetState(GameState state) { _currentState = state; }

    inline std::vector<Token*>& GetMovableTokens() { return _movableTokens; }
    inline GameState GetCurrentState() { return _currentState; }
    Token* GetSelectedToken() const { return _selectedToken; }
	int GetNewTokenPosition() const { return _newTokenPosition; }
};