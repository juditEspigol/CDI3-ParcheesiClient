#pragma once
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "GlobalFunctions.h"

const std::string TEXTURE = "../Assets/Spritesheets/button_blue.png";

class Button
{
private:
	sf::Sprite* sprite;

public:
	Button(sf::Vector2f _position = sf::Vector2f(100, 100), const sf::String& _textureFilePath = TEXTURE);
	~Button();

	void Render(sf::RenderWindow& _window)
	{
		_window.draw(*sprite);
	}

	void HandleEvent(const sf::Event& _event, sf::RenderWindow& _window, sf::TcpSocket& _socket)
	{
		if (const sf::Event::MouseButtonPressed* mousePressed = _event.getIf<sf::Event::MouseButtonPressed>())
		{
			switch (mousePressed->button)
			{
			case sf::Mouse::Button::Left:
				{
					if(sprite->getGlobalBounds().contains(sf::Vector2f(mousePressed->position)))
					{
						std::cout << "Button pressed" << std::endl;
					}
				}
				break; 
			default:
				break; 
			}
		}
	}
};

