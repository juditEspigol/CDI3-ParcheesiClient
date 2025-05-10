#pragma once

#include <memory>
#include <random>

#include "ButtonPacketSender.h"
#include "IGameStateProvider.h"

#define END_TURN_WIDTH 125
#define END_TURN_HEIGHT 40
#define END_TURN_Y_OFFSET 75
#define END_TURN_TEXT_SIZE 20
#define END_TURN_TEXT "END TURN"

class EndTurnButton : public ButtonPacketSender
{
private:
    IGameStateProvider* stateProvider;
    sf::RectangleShape _buttonBorder;

    sf::Font _buttonFont;

public:
    EndTurnButton(IGameStateProvider* provider);
    void OnLeftClick(const sf::Event::MouseButtonPressed* mousePressed, sf::TcpSocket& socket) override;
    sf::RectangleShape GetEndButton(float width, float height);
    sf::Text GetText();
    void UnselectButton() override;
};