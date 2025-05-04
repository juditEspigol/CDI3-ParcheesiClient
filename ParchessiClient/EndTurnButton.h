#pragma once

#include "ButtonPacketSender.h"
#include "IGameStateProvider.h"

#define END_TURN_WIDTH 100
#define END_TURN_HEIGHT 100
#define END_TURN_Y_POS -0.3
#define END_TURN_TEXT "END TURN"

class EndTurnButton : public ButtonPacketSender
{
private:
    IGameStateProvider* stateProvider;
    sf::RectangleShape _buttonBorder;

public:
    EndTurnButton(IGameStateProvider* provider);
    void OnLeftClick(const sf::Event::MouseButtonPressed* mousePressed, sf::TcpSocket& socket) override;
    sf::RectangleShape GetEndButton(float width, float height);

};