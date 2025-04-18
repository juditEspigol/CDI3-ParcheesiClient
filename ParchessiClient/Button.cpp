#include "Button.h"

Button::Button(sf::Vector2f _position, const sf::String& _textureFilePath)
{
	sf::Texture buttonTexture = LoadTexture(_textureFilePath);
	sprite = new sf::Sprite(buttonTexture);
	sprite->setScale(sf::Vector2f(0.20, 0.10)); 
	sprite->setPosition(_position);
}
Button::~Button()
{
	delete sprite;
}