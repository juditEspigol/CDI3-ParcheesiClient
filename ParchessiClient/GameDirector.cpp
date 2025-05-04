#include "GameDirector.h"

GameDirector::GameDirector(Table& table) :
    _table(table), 
    _currentPlayer(1), 
    _currentState(GameState::WAITING_TURN)
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
    std::cout << "PLAYER TURN -> " << _currentPlayer << std::endl;
    _movableTokens.clear();
    _selectedToken = new Token(-1, -1);
}

void GameDirector::SelectToken(sf::Vector2i mousePos)
{
    if (_currentState != GameState::DICE_ROLLED) return;

    for (Token* currentToken : _movableTokens)
    {
        sf::Vector2f distance = static_cast<sf::Vector2f>(mousePos) - currentToken->GetPosition();
        float length = std::sqrt(distance.x * distance.x + distance.y * distance.y);

        if (length <= TOKEN_RADIUS && currentToken->GetIsSelectable())
        {
            _selectedToken = currentToken;
            _currentState = GameState::PIECE_SELECTED;
            MoveSelectedToken();
        }
    }
}

void GameDirector::MoveSelectedToken()
{
    if (_currentState != GameState::PIECE_SELECTED) return;

    _table.UpdatePositions(_selectedToken->Move(_dice->GetDiceValue()));

    _currentState = GameState::TURN_COMPLETE;

    // Reset all selectable tokens
    for (Token* currentToken : _table.GetTokens())
    {
        currentToken->SetSelectable(false);
    }

    EndTurn();
}

void GameDirector::CalculateMovableTokens()
{
    if (_currentState != GameState::WAITING_TURN) 
        return;

    _currentState = GameState::DICE_ROLLED;

    for (Token* currentToken : _table.GetTokens())
    {
        if (IsTokenFromCurrentPlayer(*currentToken))
        {
            if (!currentToken->GetIsInBase())
            {
                currentToken->SetSelectable(CanTokenMove(*currentToken));
                _movableTokens.push_back(currentToken);
            }

            if (currentToken->GetIsInBase() && _dice->GetDiceValue() == 5)
            {
                currentToken->SetSelectable(CanTokenMove(*currentToken));
                _movableTokens.push_back(currentToken);
            }
        }
    }

    _dice->UnselectButton();

    if (_movableTokens.empty() && _endTurn->IsSelected())
    {
        _currentState = GameState::TURN_COMPLETE;
        EndTurn();
    }
}

bool GameDirector::CanTokenMove(Token& token)
{
    // Comprobar si el token es movible
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
    if (_currentState != GameState::TURN_COMPLETE) return;

    _endTurn->UnselectButton();
    _dice->SetDiceValue(0);

    _currentPlayer = _currentPlayer % 4 + 1;
    StartPlayerTurn(_currentPlayer);
}

bool GameDirector::IsDiceRollAllowed() const
{
    return _currentState == GameState::WAITING_TURN;
}

bool GameDirector::IsEndTurnAllowed() const
{
    return _currentState == GameState::DICE_ROLLED ||
        _currentState == GameState::PIECE_SELECTED;
}
