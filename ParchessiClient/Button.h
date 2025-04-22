#pragma once
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#include "TextFill.h"

const std::string WHITE_BOX = "../Assets/Spritesheets/whiteBox.png";
const std::string GRAY_BOX = "../Assets/Spritesheets/grayBox.png";

sf::Texture* LoadTexture(const std::string& _filePath);

class Button
{
protected:
	
	bool pressed;
	bool selected;

	sf::Texture* textureSelected;
	sf::Texture* textureNotSelected;

	sf::Sprite* sprite;
	TextFill* text; 

	void SelectButton();
	void UnselectButton();

public:
	Button(sf::Vector2f _position = sf::Vector2f(100, 100));
	~Button();

	inline bool HasBeenPressed() const { return pressed; };

	virtual void Render(sf::RenderWindow& _window);
	virtual void HandleEvent(const sf::Event& _event, sf::RenderWindow& _window, sf::TcpSocket& _socket);
};

