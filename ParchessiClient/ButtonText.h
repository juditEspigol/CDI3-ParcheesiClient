#pragma once
#include "Button.h"
#include "GlobalFunctions.h"
#include "TextFill.h"

class ButtonText
{
private:
	sf::Texture* notSelected; 
	sf::Texture* selected; 
	sf::Sprite* sprite;
	TextFill* text;

	bool textActivated; 

public:
	ButtonText(TextFill* _text, sf::Vector2f _position = sf::Vector2f(100, 300), const sf::String& _textureFilePath = TEXTURE);
	~ButtonText();

	void Render(sf::RenderWindow& _window)
	{
		_window.draw(*sprite);
		text->Render(_window);
	}

	void HandleEvent(const sf::Event& _event, sf::RenderWindow& _window, sf::TcpSocket& _socket)
	{
		if (const sf::Event::MouseButtonPressed* mousePressed = _event.getIf<sf::Event::MouseButtonPressed>())
		{
			switch (mousePressed->button)
			{
			case sf::Mouse::Button::Left:
			{
				if (sprite->getGlobalBounds().contains(sf::Vector2f(mousePressed->position)))
				{
					sprite->setTexture(*selected);
					textActivated = true;
				}
				else
				{
					sprite->setTexture(*notSelected);
					textActivated = false;
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
				sprite->setTexture(*notSelected);
				textActivated = false;
			}
			break;
			default:
				break;
			}
		}

		if(textActivated)
		{
			text->HandleEvent(_event, _window, _socket);
		}
	}
};

