#pragma once
#include "TextFill.h"
#include "TextureManager.h"

class Button
{
protected:
	
	bool pressed;
	bool selected;

	sf::Sprite* sprite;
	TextFill* text; 

	virtual void SelectButton();
	virtual void UnselectButton();

public:
	Button(sf::Vector2f _position = sf::Vector2f(100, 100));
	~Button();

	inline bool HasBeenPressed() const { return pressed; };
	inline bool IsSelected() const { return selected; };
	inline TextFill* GetText() const { return text; }

	virtual void Render(sf::RenderWindow& _window);

	virtual void OnLeftClick(const sf::Event::MouseButtonPressed* _mousePressed, sf::TcpSocket& _socket);
	virtual void OnEnter(const sf::Event::KeyPressed* _keyPressed, sf::TcpSocket& _socket);
};

