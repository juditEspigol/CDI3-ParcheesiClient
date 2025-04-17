#include <SFML/Graphics.hpp>
#include <iostream>
#include "Table.h"


#define FRAME_WIDTH 96
#define FRAME_HEIGHT 101
#define NUM_FRAMES 10
#define ANIMATION_SPEED 0.014

void UpdateSprite(sf::Sprite& animatedSprite, int& currentFrame, float& deltaTimeAnimation) {
	if (deltaTimeAnimation >= ANIMATION_SPEED) 
	{
		currentFrame = (currentFrame + 1) % NUM_FRAMES;
		animatedSprite.setTextureRect(sf::IntRect({ currentFrame * FRAME_WIDTH,0 }, { FRAME_WIDTH,FRAME_HEIGHT }));
	}
}

sf::Texture LoadSpriteSheet(const std::string& filePath) {
	sf::Texture texture;
	if (!texture.loadFromFile(filePath)) 
	{
		std::cerr << "ERROR LOADING SPRITE SHIT" << std::endl;
		return texture;
	}

	return texture;
}

void Render(sf::RenderWindow& window, sf::Sprite& sprite)
{
	window.draw(sprite);
}

void HandleEvent(const sf::Event& event, sf::RenderWindow& window, Table* table) {
	if (event.is < sf::Event::Closed>()) {
		window.close();
	}
	if (const sf::Event::KeyPressed* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
		switch (keyPressed->code) {
		case sf::Keyboard::Key::Escape:
			window.close();
			break;
		default:
			break;
		}
	}
	if (const sf::Event::MouseButtonPressed* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
		switch (mousePressed->button) {
		case sf::Mouse::Button::Left:
			for (Token* a : table->GetTokens())
			{
				sf::Vector2f distance = static_cast<sf::Vector2f>(mousePressed->position) - a->GetPosition();
				float length = std::sqrt(distance.x * distance.x + distance.y * distance.y);
				std::cout << length << std::endl;
				if (length <= TOKEN_RADIUS)
				{
					a->Move(1);
				}
			}
		}
	}
}

void main() {

	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode({ WIDTH ,HEIGHT}), "TUTORIAL");
	sf::Clock deltaTimeClock;
	float deltaTimeAnimation = 0.0f;
	int currentFrame = 0;

	sf::Texture spriteSheet = LoadSpriteSheet("../Assets/Spritesheets/ParchisTable.png");

	sf::Sprite sprite = sf::Sprite(spriteSheet);

	sf::Vector2f newOrigin = sf::Vector2f(sprite.getTexture().getSize().x * 0.5, sprite.getTexture().getSize().y * 0.5);

	sf::Vector2f newPosition = sf::Vector2f(WIDTH * 0.5f, HEIGHT * 0.5f);

	Table* table = new Table();

	while (window->isOpen()) 
	{
		float deltaTime = deltaTimeClock.restart().asSeconds();
		deltaTimeAnimation = deltaTime;
		while (const std::optional event = window->pollEvent()) {
			//aqui va lo que quiero que ocurra si hay un input/evento
			HandleEvent(*event, *window, table);
		}
		
		table->Update();

		window->clear();

		Render(*window, sprite);
		table->Draw(*window);

		window->display();
	}
	delete window;
}