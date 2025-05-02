#include "ButtonTextUpdater.h"

ButtonTextUpdater::ButtonTextUpdater(const std::string _textDefault, sf::Vector2f _position)
	: Button(_position)
{
	sprite->setScale(sf::Vector2f(0.30, 0.05));
	sprite->setPosition(_position);

	text = new TextFill(_textDefault, sf::Vector2f(_position.x + 10, _position.y + 5));
}

ButtonTextUpdater::~ButtonTextUpdater()
{
}

void ButtonTextUpdater::OnLeftClick(const sf::Event::MouseButtonPressed* _mousePressed, sf::TcpSocket& _socket)
{
	if (!selected && sprite->getGlobalBounds().contains(sf::Vector2f(_mousePressed->position)))
	{
		SelectButton();
	}
	else if (selected)
	{
		UnselectButton();
	}
}
