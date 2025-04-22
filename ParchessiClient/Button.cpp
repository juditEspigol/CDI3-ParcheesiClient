#include "Button.h"

sf::Texture* LoadTexture(const std::string& _filePath)
{
	sf::Texture* tempTexture = new sf::Texture();
	if (!tempTexture->loadFromFile(_filePath))
	{
		std::cerr << "ERROR LOADING TEXTURE: " << _filePath << std::endl;
		return nullptr;
	}

	return tempTexture;
}

void Button::SelectButton()
{
	sprite->setTexture(*textureSelected);
	selected = true;
}

void Button::UnselectButton()
{
	sprite->setTexture(*textureNotSelected);
	selected = false;
}

Button::Button(sf::Vector2f _position)
	: pressed(false), selected(false)
{
	textureSelected = LoadTexture(WHITE_BOX);
	textureNotSelected = LoadTexture(GRAY_BOX);
	sprite = new sf::Sprite(*textureNotSelected);
	
	sprite->setScale(sf::Vector2f(0.2, 0.05)); 
	sprite->setPosition(_position);
}

Button::~Button()
{
}

void Button::Render(sf::RenderWindow& _window)
{
	_window.draw(*sprite);
	text->Render(_window);
}

void Button::HandleEvent(const sf::Event& _event, sf::RenderWindow& /*_window*/, sf::TcpSocket& /*_socket*/)
{
	if (const sf::Event::MouseButtonPressed* mousePressed = _event.getIf<sf::Event::MouseButtonPressed>())
	{
		switch (mousePressed->button)
		{
		case sf::Mouse::Button::Left:
		{
			if (sprite->getGlobalBounds().contains(sf::Vector2f(mousePressed->position)))
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
