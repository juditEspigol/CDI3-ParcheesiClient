#include "EndTurnButton.h"

EndTurnButton::EndTurnButton(IGameStateProvider* provider) :
	ButtonPacketSender(END_TURN, {}, sf::Vector2f(0, 0)),
    stateProvider(provider)
{
    _buttonBorder.setScale(sf::Vector2f(END_TURN_WIDTH, END_TURN_HEIGHT));
    _buttonBorder.setOrigin(sf::Vector2f(END_TURN_WIDTH * 0.5f, END_TURN_HEIGHT * 0.5f));
    _buttonBorder.setPosition(sf::Vector2f(0, 0));

    _buttonBorder.setOutlineThickness(2.0f);
    _buttonBorder.setOutlineColor(sf::Color::Black);
    _buttonBorder.setFillColor(sf::Color::White);
}

void EndTurnButton::OnLeftClick(const sf::Event::MouseButtonPressed* mousePressed, sf::TcpSocket& socket)
{
    if (!stateProvider || !stateProvider->IsEndTurnAllowed())
        return;

    if (_buttonBorder.getGlobalBounds().contains(sf::Vector2f(mousePressed->position)))
    {
        sf::Packet tempPacket;
        tempPacket << packetType;

        std::cout << "END TURN PRESSED" << std::endl;

        pressed = true;
        SendData(socket, tempPacket);
    }
}

sf::RectangleShape EndTurnButton::GetEndButton(float width, float height)
{
    sf::RectangleShape button = _buttonBorder;
    _buttonBorder.setPosition(sf::Vector2f(width * 0.5f, height * 0.5f));

    button.setFillColor(sf::Color::Black);

    return button;
}
