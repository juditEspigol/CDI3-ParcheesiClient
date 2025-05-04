#pragma once
class IGameStateProvider
{
public:
    virtual ~IGameStateProvider() = default;
    virtual bool IsDiceRollAllowed() const = 0;
    virtual bool IsEndTurnAllowed() const = 0;
};

