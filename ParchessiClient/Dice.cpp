#include "Dice.h"

Dice::Dice(IGameStateProvider* provider) :
    ButtonPacketSender(DICE_ROLL, {}, sf::Vector2f(0, 0)),
    stateProvider(provider),
    _diceValue(0),
    _rng(std::random_device{}())
{
    _turnIndicator.setSize(sf::Vector2f(PLAYER_INDICATOR_SIZE, PLAYER_INDICATOR_SIZE));
    _turnIndicator.setOrigin(sf::Vector2f(PLAYER_INDICATOR_SIZE * 0.5f, PLAYER_INDICATOR_SIZE * 0.5f));
    _turnIndicator.setPosition(sf::Vector2f(0, 0));
    _turnIndicator.setOutlineThickness(2.0f);
    _turnIndicator.setOutlineColor(sf::Color::Black);

    if (!_font.openFromFile("../Assets/Fonts/Dice.ttf"))
    {
        std::cerr << "Error cargando fuente" << std::endl;
    }
}

void Dice::OnLeftClick(const sf::Event::MouseButtonPressed* _mousePressed, sf::TcpSocket& _socket)
{
    if (!stateProvider->IsDiceRollAllowed()) 
        return;

    if (_turnIndicator.getGlobalBounds().contains(sf::Vector2f(_mousePressed->position)))
    {
        sf::Packet tempPacket;

        RollDice();

        tempPacket << GetDiceValue();

        selected = true;
        SendData(_socket, tempPacket);
    }
}

void Dice::RollDice()
{
    std::uniform_int_distribution<int> dist(1, 6);
    _diceValue = dist(_rng);

    std::cout << "Dice Value = " << _diceValue << std::endl;
}

void Dice::ForceDiceValue(int value)
{
    _diceValue = value;
    
    std::cout << "Forced Dice Value = " << _diceValue << std::endl;
}

sf::Text Dice::GetDiceText()
{
    sf::Text text(_font);

    text.setFont(_font);
    text.setCharacterSize(DICE_INDICATOR_SIZE);
    text.setFillColor(sf::Color::Black);
    text.setStyle(sf::Text::Bold);

    text.setString(std::to_string(_diceValue));

    sf::FloatRect textRect = text.getLocalBounds();

    text.setOrigin(sf::Vector2f(
        textRect.position.x + textRect.size.x / 2.0f,
        textRect.position.y + textRect.size.y / 2.0f
    ));

    text.setPosition(_turnIndicator.getPosition());
    return text;
}

sf::RectangleShape Dice::GetTurnIndicator(int currentPlayer, float width, float height)
{
    sf::RectangleShape indicator = _turnIndicator;
    _turnIndicator.setPosition(sf::Vector2f(width * 0.5f, height * 0.5f));

    switch (currentPlayer) {
    case 1:
        indicator.setFillColor(sf::Color::Blue);
        break;
    case 2:
        indicator.setFillColor(sf::Color::Red);
        break;
    case 3:
        indicator.setFillColor(sf::Color::Green);
        break;
    case 4:
        indicator.setFillColor(sf::Color::Yellow);
        break;
    default:
        indicator.setFillColor(sf::Color::White);
        break;
    }

    return indicator;
}

void Dice::UnselectButton()
{
    selected = false;
}
