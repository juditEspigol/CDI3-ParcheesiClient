#include "EndTurnButton.h"

EndTurnButton::EndTurnButton(IGameStateProvider* provider) :
	ButtonPacketSender(END_TURN, {}, sf::Vector2f(0, 0)),
    stateProvider(provider)
{
    _buttonBorder.setSize(sf::Vector2f(END_TURN_WIDTH, END_TURN_HEIGHT));
    _buttonBorder.setPosition(sf::Vector2f(0, 0));
	_buttonBorder.setOrigin(_buttonBorder.getSize() / 2.0f);

    if (!_buttonFont.openFromFile("../Assets/Fonts/Arial.ttf"))
    {
        std::cerr << "Error cargando fuente" << std::endl;
    }

    _buttonBorder.setOutlineThickness(4.0f);
    _buttonBorder.setOutlineColor(sf::Color::Black);
    _buttonBorder.setFillColor(sf::Color::White);
}

void EndTurnButton::OnLeftClick(const sf::Event::MouseButtonPressed* mousePressed, sf::TcpSocket& socket)
{
    if (!stateProvider || !stateProvider->IsEndTurnAllowed() /* || CLIENT_MANAGER.GetSelfID() != stateProvider->GetCurrentPlayer() */)
        return;

    if (_buttonBorder.getGlobalBounds().contains(sf::Vector2f(mousePressed->position)))
    {
        sf::Packet tempPacket;

        std::cout << "END TURN PRESSED" << std::endl;
        selected = true;

        for (Client* client : CLIENT_MANAGER.GetClients())
        {
            tempPacket << packetType;
            tempPacket << selected;
            NETWORK_MANAGER.SendData(*client->GetSocket(), tempPacket);
            tempPacket.clear();
        }
    }
}

sf::RectangleShape EndTurnButton::GetEndButton(float width, float height)
{
    _buttonBorder.setPosition(sf::Vector2f(width * 0.5f, (height * 0.5f) + END_TURN_Y_OFFSET));

    return _buttonBorder;
}

sf::Text EndTurnButton::GetText()
{
    sf::Text text(_buttonFont);

    text.setFont(_buttonFont);
    text.setCharacterSize(END_TURN_TEXT_SIZE);
    text.setFillColor(sf::Color::Black);
    text.setStyle(sf::Text::Bold);

    text.setString(END_TURN_TEXT);

    sf::FloatRect textRect = text.getLocalBounds();

    text.setOrigin(sf::Vector2f(
        textRect.position.x + textRect.size.x / 2.0f,
        textRect.position.y + textRect.size.y / 2.0f
    ));

    text.setPosition(_buttonBorder.getPosition());
    return text;
}

void EndTurnButton::UnselectButton()
{
    selected = false;
}