#pragma once
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

const std::string ROBOTO_MEDIUM = "../Assets/Fonts/Roboto-Condensed.ttf";
const std::string ROBOTO_BOLD = "../Assets/Fonts/Roboto-BoldCondensed.ttf";
const int TEXT_SIZE = 24; 

class TextFill
{
private:
	sf::Font font; 
	sf::Text* textBox; 

	std::string defaultText;
	std::string content; 

	inline bool HasContent() const { return content != defaultText; }

public:
	TextFill(const sf::String& _textDefault, sf::Vector2f _position = sf::Vector2f(200, 200));
	~TextFill();

	inline std::string GetContent() const { return HasContent() ? content : ""; }
	inline void SetPosition(const sf::Vector2f _position) { textBox->setPosition(_position); }
	inline void SetColor(const sf::Color _color) { textBox->setFillColor(_color); }
	inline void SetBoldFont(const bool _activate = true)
	{
		_activate? font.openFromFile(ROBOTO_BOLD) : font.openFromFile(ROBOTO_MEDIUM);
	}

	void Render(sf::RenderWindow& _window); 

	void InsertChar(const sf::Event::TextEntered* _textEntered);
	void RemoveChar();
};