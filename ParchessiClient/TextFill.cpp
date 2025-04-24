#include "TextFill.h"

TextFill::TextFill(const sf::String& _textDefault, sf::Vector2f _position)
{
	font.openFromFile(ROBOTO_MEDIUM);

	textBox = new sf::Text(font);
	textBox->setCharacterSize(TEXT_SIZE);
	textBox->setFillColor(sf::Color::Gray);

	defaultText = _textDefault;
	content = _textDefault;

	textBox->setString(content);
	textBox->setPosition(_position);
}

TextFill::~TextFill()
{
	//delete textBox;
}

void TextFill::InsertChar(const sf::Event::TextEntered* _textEntered)
{
	if (_textEntered->unicode < 128 && _textEntered->unicode != 8 && _textEntered->unicode != 13)
	{
		if (content == defaultText)
		{
			textBox->setFillColor(sf::Color::Black);
			content = _textEntered->unicode;
		}
		else
		{
			content += static_cast<char>(_textEntered->unicode); // Add new char
		}
		textBox->setString(content); // Update
	}
}

void TextFill::RemoveChar()
{
	if (content.length() > 1) // Remove char from content
	{
		if (content != defaultText)
		{
			content.pop_back();
			textBox->setString(content);
		}
	}
	else // Set default text because there's no content
	{
		textBox->setFillColor(sf::Color::Gray);
		content = defaultText;
		textBox->setString(content);
	}
}

void TextFill::Render(sf::RenderWindow& _window)
{
	_window.draw(*textBox);
}

void TextFill::HandleEvent(const sf::Event& _event, sf::RenderWindow& /*_window*/, sf::TcpSocket& /*_socket*/)
{
	if (const sf::Event::TextEntered* textEntered = _event.getIf<sf::Event::TextEntered>())
	{
		InsertChar(textEntered);
	}

	if (const sf::Event::KeyPressed* keyPressed = _event.getIf<sf::Event::KeyPressed>())
	{
		switch (keyPressed->code)
		{
		case sf::Keyboard::Key::Backspace:
			RemoveChar();
		break;
		default:
			break;
		}
	}
}