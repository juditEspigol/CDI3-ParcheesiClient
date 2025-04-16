#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include <string>

#define SERVER_PORT 55000 // puertos abiertos del 55000 - 55050

#define WIDTH 1280
#define HEIGHT 720

#define FRAME_WIDTH 96
#define FRAME_HEIGHT 101
#define NUM_FRAMES 10
#define ANIMATION_SPEED 0.014

const sf::IpAddress SERVER_IP = sf::IpAddress(79, 152, 211, 184);//sf::IpAddress(10, 40, 2, 183); // Loopback

enum packetType { LOGIN, RESGISTER };

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

void HandleEvent(const sf::Event& _event, sf::RenderWindow& _window) 
{
	if (_event.is < sf::Event::Closed>())
	{
		_window.close();
	}
	if (const sf::Event::TextEntered* textEntered = _event.getIf<sf::Event::TextEntered>())
	{
		if (textEntered->unicode < 128)
		{
			std::cout << (char)textEntered->unicode;
		}
	}
	if (const sf::Event::KeyPressed* keyPressed = _event.getIf<sf::Event::KeyPressed>())
	{
		switch (keyPressed->code) 
		{
		case sf::Keyboard::Key::Escape:
			_window.close();
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

	sf::Texture waitingTexture = LoadSpriteSheet("../Assets/Spritesheets/Waiting.png");
	sf::Sprite waitingSprite = sf::Sprite(waitingTexture);

	sf::Texture spriteSheetTexture = LoadSpriteSheet("../Assets/Spritesheets/S_link.png");
	sf::Sprite animatedSprite = sf::Sprite(spriteSheetTexture);

	sf::Font robotoFont;
	robotoFont.openFromFile("../Assets/Fonts/Roboto-Medium.ttf");

	sf::Text text(robotoFont);
	text.setString("Waiting To Connect"); 
	text.setCharacterSize(24); // in pixels, not points!

	// set the color
	text.setFillColor(sf::Color::Red);

	// set the text style
	text.setStyle(sf::Text::Bold | sf::Text::Underlined);

	sf::SoundBuffer buffer("../Assets/FX_Click_Button_01.mp3");
	sf::Sound sound(buffer);
	sound.play();

	Render(*window, waitingSprite);


	window->draw(text);
	window->display();

	if (socket.connect(SERVER_IP, SERVER_PORT) != sf::Socket::Status::Done)
	{
		std::cerr << "Error al connectar con el servidor." << std::endl; 
	}
	else
	{
		while (window->isOpen())
		{
			//std::cout << "Connectado con el servidor" << std::endl;

			// TCP
			sf::sleep(sf::seconds(1));
			std::string message, username, password;
			//std::cout << "Inserta mensaje para el servidor, -1 para salir" << std::endl;

			// HANDLE INPUT EVENT
			float deltaTime = deltaTimeClock.restart().asSeconds();
			deltaTimeAnimation = deltaTime;
			while (const std::optional event = window->pollEvent())
			{
				HandleEvent(*event, *window);
			}
			UpdateSprite(animatedSprite, currentFrame, deltaTimeAnimation);
			Render(*window, animatedSprite);

			std::cin >> message;
			username = "Judith";
			password = "Espigol";

			if (message == "-1")
			{
				std::cout << "Desconectado..." << std::endl;
				window->close();
			}
			else
			{
				sf::Packet packet;
				packet << packetType::LOGIN << message;
				SendData(socket, packet);
			}

		}
	}

	socket.disconnect(); 

	std::cout << "Desconectado del servidor" << std::endl;

	delete window;
}