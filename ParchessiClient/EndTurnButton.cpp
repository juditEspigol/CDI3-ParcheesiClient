#include "EndTurnButton.h"

EndTurnButton::EndTurnButton(IGameStateProvider* provider) :
	ButtonPacketSender(END_TURN, {}, sf::Vector2f(0, 0)),
    stateProvider(provider)
{
    _buttonBorder.setSize(sf::Vector2f(100, 50));
    _buttonBorder.setPosition(sf::Vector2f(0, 0));

    _buttonBorder.setOutlineThickness(2.0f);
    _buttonBorder.setOutlineColor(sf::Color::Magenta);
    _buttonBorder.setFillColor(sf::Color::Magenta);
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
    _buttonBorder.setPosition(sf::Vector2f(width * 0.75f, height * 0.75f)); // Cambio de posición para hacerlo más visible

    return _buttonBorder;
}

void EndTurnButton::UnselectButton()
{
    selected = false;
}