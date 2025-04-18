#pragma once
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

const std::string FONT = "../Assets/Fonts/Roboto-Medium.ttf";

class TextFill
{
private:
	sf::Font font; 
	sf::Text* textBox; 
	std::string defaultText;
	std::string content; 

public:
	TextFill(const sf::String& _text, sf::Vector2f _position = sf::Vector2f(200, 200))
	{
		font.openFromFile(FONT);

		textBox = new sf::Text(font);
		textBox->setCharacterSize(24);
		textBox->setFillColor(sf::Color::Gray);
		
		defaultText = _text;
		content = _text; 
		textBox->setString(content);
		textBox->setPosition(_position);
	}
	~TextFill()
	{
		delete textBox;
	}
	void SetPosition(sf::Vector2f _position)
	{
		textBox->setPosition(_position);
	}

	void Render(sf::RenderWindow& _window)
	{
		_window.draw(*textBox);
	}

	void HandleEvent(const sf::Event& _event, sf::RenderWindow& _window, sf::TcpSocket& _socket)
	{
		if (const sf::Event::TextEntered* textEntered = _event.getIf<sf::Event::TextEntered>())
		{
			if (textEntered->unicode < 128 
				&& textEntered->unicode != 8 && textEntered->unicode != 13)
			{
				if (content == defaultText)
				{
					textBox->setFillColor(sf::Color::Black);
					content = textEntered->unicode;
				}
				else
				{
					content += static_cast<char>(textEntered->unicode); // Add new char
				}
				textBox->setString(content); // Update
			}
		}
		if (const sf::Event::KeyPressed* keyPressed = _event.getIf<sf::Event::KeyPressed>())
		{
			switch (keyPressed->code)
			{
			case sf::Keyboard::Key::Enter:
			{
				if (content != defaultText)
				{
					std::cout << "Send Message: " << content << std::endl;
					//sf::Packet packet;
					//packet << packetType::LOGIN << content;
					//SendData(_socket, packet);
				}
			}
			break;
			case sf::Keyboard::Key::Backspace:
			{
				if(content.length() > 1)
				{
					if(content != defaultText)
					{
						content.pop_back();
						textBox->setString(content);
					}
				}
				else
				{
					textBox->setFillColor(sf::Color::Gray);
					content = defaultText;
					textBox->setString(content);	
				}
			}
			break;
			default:
				break;
			}
		}
	}
};