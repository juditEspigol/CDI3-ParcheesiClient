#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "Button.h"
#include "ButtonText.h"
#include "TextFill.h"

#define SERVER_PORT 55000 // puertos abiertos del 55000 - 55050

#define WIDTH 1280
#define HEIGHT 720

#define FRAME_WIDTH 96
#define FRAME_HEIGHT 101
#define NUM_FRAMES 10
#define ANIMATION_SPEED 0.014

const sf::IpAddress SERVER_IP = sf::IpAddress(127, 152, 211, 1);//sf::IpAddress(10, 40, 2, 183); // Loopback /// 79, 152, 211, 184

enum packetType { LOGIN, RESGISTER, ROOM };

void UpdateSprite(sf::Sprite& animatedSprite, int& currentFrame, float& deltaTimeAnimation) 
{
	if (deltaTimeAnimation >= ANIMATION_SPEED) 
	{
		currentFrame = (currentFrame + 1) % NUM_FRAMES;
		animatedSprite.setTextureRect(sf::IntRect({ currentFrame * FRAME_WIDTH,0 }, { FRAME_WIDTH,FRAME_HEIGHT }));
	}
}

void HandShake(sf::Packet _data)
{
	std::string receivedMessage; 
	_data >> receivedMessage; // Sacar el mensaje del packet

	std::cout << "Mensaje recivido del servidor: `" << receivedMessage << "`" << std::endl;
}

sf::Texture LoadSpriteSheet(const std::string& _filePath)
{
	sf::Texture texture;
	if (!texture.loadFromFile(_filePath))
	{
		std::cerr << "ERROR LOADING SPRITE SHIT" << std::endl;
	}

	return texture;
}

void Render(sf::RenderWindow& _window, sf::Sprite& _sprite)
{
	_window.clear(sf::Color::Black);
	_window.draw(_sprite);
	_window.display();
}

void WriteText(const sf::Event& _event, sf::RenderWindow& _window, sf::Text& _text)
{
	if (const sf::Event::TextEntered* textEntered = _event.getIf<sf::Event::TextEntered>())
	{
		if (textEntered->unicode < 128)
		{
			std::string tempText = _text.getString(); // Actual text
			tempText += (char)textEntered->unicode; // Add new char
			
			_text.setString(tempText); // Update
			_window.draw(_text);
			_window.display();
		}
	}
}

void SendData(sf::TcpSocket& _clientSocket, sf::Packet& _packet);

void HandleEvent(const sf::Event& _event, sf::RenderWindow& _window, sf::TcpSocket& _socket, sf::Text& _text)
{
	if (_event.is < sf::Event::Closed>())
	{
		_window.close();
	}
	if (const sf::Event::KeyPressed* keyPressed = _event.getIf<sf::Event::KeyPressed>())
	{
		switch (keyPressed->code) 
		{
		case sf::Keyboard::Key::Escape:
			std::cout << "Disconected..." << std::endl;
			_window.close();
			break;
		case sf::Keyboard::Key::Enter:
			{
				sf::Packet packet;
				packet << packetType::LOGIN << _text.getString();
				SendData(_socket, packet);
			}
			break;
		case sf::Keyboard::Key::Backspace:
		{
			std::string tempText = _text.getString(); // Actual text
			tempText = tempText.substr(0, tempText.length() -1);

			_text.setString(tempText); // Update
			_window.draw(_text);
			_window.display();
		}
			break; 
		default:
			break;
		}
	}
	if (const sf::Event::MouseButtonPressed* mousePressed = _event.getIf<sf::Event::MouseButtonPressed>())
	{
		switch (mousePressed->button) 
		{
		case sf::Mouse::Button::Left:
			std::cout << "Pressed at: " << mousePressed->position.x << " " << mousePressed->position.y << std::endl;
		}
	}
}

sf::Packet& operator>>(sf::Packet& _packet, packetType& _type) 
{
	int temp;
	_packet >> temp;
	_type = static_cast<packetType>(temp);

	return _packet;
};

void SendData(sf::TcpSocket& _clientSocket, sf::Packet& _packet)
{
	if (_clientSocket.send(_packet) != sf::Socket::Status::Done)
	{
		std::cerr << "Error al enviar el paquete al servidor" << std::endl;
	}
	else
	{
		std::cout << "Mensaje enviado" << std::endl;
	}
	_packet.clear();
}

void main()
{
	// TCP
	sf::TcpSocket socket; 

	// Render SFML
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode({ WIDTH, HEIGHT }), "Client");
	sf::Clock deltaTimeClock;
	float deltaTimeAnimation = 0.f;
	int currentFrame = 0;

	Button button;
	ButtonText buttonText(new TextFill("Insert user..."));
	ButtonText buttonText2(new TextFill("Insert password..."), sf::Vector2f(100, 350));
	TextFill userText("Insert User...");

	//sf::SoundBuffer buffer("../Assets/Sounds/FX_Click_Button_01.mp3");
	//sf::Sound sound(buffer);
	//sound.play();

	if (socket.connect(SERVER_IP, SERVER_PORT) != sf::Socket::Status::Done)
	{
		std::cerr << "Error connecting to server." << std::endl; 
	}
	else
	{
		std::cout << "Connected to server" << std::endl;
		while (window->isOpen())
		{
			// TCP
			std::string message, username, password;

			// HANDLE INPUT EVENT
			//float deltaTime = deltaTimeClock.restart().asSeconds();
			//deltaTimeAnimation = deltaTime;
			while (const std::optional event = window->pollEvent())
			{
				button.HandleEvent(*event, *window, socket);
				buttonText.HandleEvent(*event, *window, socket);
				buttonText2.HandleEvent(*event, *window, socket);
				userText.HandleEvent(*event, *window, socket);
			}
			window->clear(sf::Color::Black);
			buttonText.Render(*window);
			buttonText2.Render(*window);
			userText.Render(*window);
			button.Render(*window);
			window->display();
		}
	}

	socket.disconnect(); 

	std::cout << "Desconectado del servidor" << std::endl;

	delete window;
}