#pragma once
#include "Button.h"

class ButtonTextUpdater : public Button
{
private:

public:
	ButtonTextUpdater(const std::string _defaultText, sf::Vector2f _position = sf::Vector2f(100, 100));
	~ButtonTextUpdater();

	inline std::string GetTextContent() const { return text->GetContent(); }

	void OnLeftClick(const sf::Event::MouseButtonPressed* _mousePressed, sf::TcpSocket& _socket) override;
};

