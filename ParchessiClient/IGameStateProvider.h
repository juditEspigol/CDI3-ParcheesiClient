#pragma once
class IGameStateProvider
{
public:
    virtual ~IGameStateProvider() = default;
    virtual bool IsDiceRollAllowed() const = 0;
	virtual bool IsTokenMoveAllowed() const = 0;
    virtual bool IsEndTurnAllowed() const = 0;
	virtual int GetCurrentPlayer() const = 0;
    virtual int GetDiceValue() const = 0;
};
