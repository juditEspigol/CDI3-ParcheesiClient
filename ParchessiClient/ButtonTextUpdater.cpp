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

void ButtonTextUpdater::HandleEvent(const sf::Event& _event, sf::RenderWindow& _window, sf::TcpSocket& _socket)
{
	if (const sf::Event::MouseButtonPressed* mousePressed = _event.getIf<sf::Event::MouseButtonPressed>())
	{
		switch (mousePressed->button)
		{
		case sf::Mouse::Button::Left:
		{
			if (sprite->getGlobalBounds().contains(sf::Vector2f(mousePressed->position)))
			{
				SelectButton();
			}
			else
			{
				UnselectButton();
			}
		}
		break;
		default:
			break;
		}
	}
	if (const sf::Event::KeyPressed* keyPressed = _event.getIf<sf::Event::KeyPressed>())
	{
		switch (keyPressed->code)
		{
		case sf::Keyboard::Key::Enter:
		{
			UnselectButton();
		}
		break;
		default:
			break;
		}
	}

	if (selected)
	{
		text->HandleEvent(_event, _window, _socket);
	}
}
