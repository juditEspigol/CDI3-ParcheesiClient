#pragma once
#include "Button.h"

class ButtonTextUpdater : public Button
{
private:

public:
	ButtonTextUpdater(const std::string _defaultText, sf::Vector2f _position = sf::Vector2f(100, 100));
	~ButtonTextUpdater();

	inline std::string GetTextContent() const { return text->GetContent(); }

	void HandleEvent(const sf::Event& _event, sf::RenderWindow& _window, sf::TcpSocket& _socket) override; 
};

