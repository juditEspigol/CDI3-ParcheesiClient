#include "ButtonText.h"

ButtonText::ButtonText(TextFill* _text, sf::Vector2f _position, const sf::String& _textureFilePath)
{
	selected = new sf::Texture();
	if (!selected->loadFromFile("../Assets/Spritesheets/whiteBox.png"))
	{
		std::cerr << "No se pudo cargar la textura" << std::endl;
	}
	notSelected = new sf::Texture();
	if (!notSelected->loadFromFile("../Assets/Spritesheets/grayBox.png"))
	{
		std::cerr << "No se pudo cargar la textura" << std::endl;
	}

	sprite = new sf::Sprite(*notSelected);
	sprite->setScale(sf::Vector2f(0.30, 0.05));
	sprite->setPosition(_position);

	text = _text;
	text->SetPosition(sf::Vector2f(_position.x + 10, _position.y + 5));
	textActivated = false;
}

ButtonText::~ButtonText()
{
	delete sprite;
}
