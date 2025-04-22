#include "SceneManager.h"

#define SERVER_PORT 55000 // puertos abiertos del 55000 - 55050

#define WIDTH 1280
#define HEIGHT 720

#define FRAME_WIDTH 96
#define FRAME_HEIGHT 101
#define NUM_FRAMES 10
#define ANIMATION_SPEED 0.014

const sf::IpAddress SERVER_IP = sf::IpAddress(127, 152, 211, 1);//sf::IpAddress(10, 40, 2, 183); // Loopback /// 79, 152, 211, 184

void HandShake(sf::Packet _data)
{
	std::string receivedMessage; 
	_data >> receivedMessage; // Sacar el mensaje del packet

	std::cout << "Mensaje recivido del servidor: `" << receivedMessage << "`" << std::endl;
}

sf::Packet& operator>>(sf::Packet& _packet, PacketType& _type) 
{
	int temp;
	_packet >> temp;
	_type = static_cast<PacketType>(temp);

	return _packet;
};

void main()
{
	SCENE_MANAGER.AddScene(AUTHENTICATION, new AuthenticateScene()); 
	SCENE_MANAGER.AddScene(CODE_ROOM, new RoomScene()); 

	SCENE_MANAGER.SetCurrentScene(AUTHENTICATION);
	SCENE_MANAGER.GetCurrentScene()->OnEnter();

	// TCP
	sf::TcpSocket socket; 

	// Render SFML
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode({ WIDTH, HEIGHT }), "Client");
	sf::Clock deltaTimeClock;

	if (socket.connect(SERVER_IP, SERVER_PORT) != sf::Socket::Status::Done)
	{
		std::cerr << "Error connecting to server." << std::endl; 
	}
	else
	{
		std::cout << "Connected to server" << std::endl;

		while (window->isOpen())
		{
			// LISTENER
			while (const std::optional event = window->pollEvent())
			{
				SCENE_MANAGER.GetCurrentScene()->HandleEvent(*event, *window, socket);
			}

			// UPDATE

			// DRAW
			SCENE_MANAGER.GetCurrentScene()->Render(*window);

			// TIME CONTROL 
			
			// CHANGE SCENE
			if (SCENE_MANAGER.GetCurrentScene()->GetIsFinished())
			{
				std::cout << "Change scene!" << std::endl;
				std::string nextScene = SCENE_MANAGER.GetCurrentScene()->OnExit();
				SCENE_MANAGER.SetCurrentScene(nextScene);
				SCENE_MANAGER.GetCurrentScene()->OnEnter();
			}
		}
	}
	socket.disconnect();
	std::cout << "Desconectado del servidor" << std::endl;

	delete window;
}