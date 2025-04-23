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
    _selectedToken = new Token(-1, -1);
}

void GameDirector::RollDice()
{
    if (_currentState != GameState::WAITING_TURN) return;

    std::uniform_int_distribution<int> dist(1, 6);
    _diceValue = dist(_rng);

    std::cout << "Dice Value = " << _diceValue << std::endl;

    _currentState = GameState::DICE_ROLLED;

    CalculateMovableTokens();

    if (_movableTokens.empty())
    {
        //EndTurn();
    }
}

void GameDirector::ForceDiceValue(int value)
{
    _diceValue = value;

    std::cout << "Forced Dice Value = " << _diceValue << std::endl;

    _currentState = GameState::DICE_ROLLED;

    CalculateMovableTokens();

    if (_movableTokens.empty())
    {
        //EndTurn();
    }
}

void GameDirector::SelectToken(sf::Vector2i mousePos)
{
    if (_currentState != GameState::DICE_ROLLED) return;

    for (Token* currentToken : _movableTokens)
    {
        sf::Vector2f distance = static_cast<sf::Vector2f>(mousePos) - currentToken->GetPosition();
        float length = std::sqrt(distance.x * distance.x + distance.y * distance.y);
        //std::cout << "Distance between mouse and token: " << length << std::endl;
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

    _table.UpdatePositions(_selectedToken->Move(_diceValue));

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
    for (Token* currentToken : _table.GetTokens())
    {
        if (IsTokenFromCurrentPlayer(*currentToken))
        {
            if (_diceValue == 5)
            {
                if (currentToken->GetIsInBase())
                {
                    currentToken->SetSelectable(CanTokenMove(*currentToken));
                    _movableTokens.push_back(currentToken);
                }
            }
            else
            {
                if (!currentToken->GetIsInBase())
                {
                    currentToken->SetSelectable(CanTokenMove(*currentToken));
                    _movableTokens.push_back(currentToken);
                }               
            }
           
        }
    }

    if (_movableTokens.empty())
    {
        std::cout << "No hay movimientos disponibles" << std::endl;
        EndTurn();
    }

    
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
    if (_currentState != GameState::TURN_COMPLETE) return;

    _currentPlayer = _currentPlayer % 4 + 1;
    StartPlayerTurn(_currentPlayer);

    _currentState = GameState::WAITING_TURN;
}
