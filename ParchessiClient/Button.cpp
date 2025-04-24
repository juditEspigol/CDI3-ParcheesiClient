#include "Button.h"

void Button::SelectButton()
{
	sprite->setTexture(*TEXTURE_MANAGER.GetSelected());
	selected = true;
}

void Button::UnselectButton()
{
	sprite->setTexture(*TEXTURE_MANAGER.GetNotSelected());
	selected = false;
}

Button::Button(sf::Vector2f _position)
	: pressed(false), selected(false)
{
	sprite = new sf::Sprite(*TEXTURE_MANAGER.GetNotSelected());
	
	sprite->setScale(sf::Vector2f(0.2, 0.05)); 
	sprite->setPosition(_position);
}

Button::~Button()
{
}

void Button::Render(sf::RenderWindow& _window)
{
	_window.draw(*sprite);
	text->Render(_window);
}

void Button::OnLeftClick(const sf::Event::MouseButtonPressed* _mousePressed, sf::TcpSocket& /*_socket*/)
{
	if (sprite->getGlobalBounds().contains(sf::Vector2f(_mousePressed->position)))
	{
		std::cout << "Button pressed with Left Click" << std::endl;
	}
}

void Button::OnEnter(const sf::Event::KeyPressed* /*_keyPressed*/, sf::TcpSocket& /*_socket*/)
{
	if (selected)
	{
		std::cout << "Pressed Enter while selected" << std::endl;
		UnselectButton();
	}
}
