#include "SceneManager.h"

#define SERVER_PORT 55000 // puertos abiertos del 55000 - 55050

#define WIDTH 960
#define HEIGHT 960

// FOR TESTING
bool testingGameplay = false;

const sf::IpAddress SERVER_IP = sf::IpAddress(79, 152, 140, 103);//sf::IpAddress(10, 40, 2, 183); // Loopback /// 79, 152, 211, 184

void HandShake(sf::Packet _data)
{
	std::string receivedMessage; 
	_data >> receivedMessage; // Sacar el mensaje del packet

	std::cout << "Mensaje recivido del servidor: `" << receivedMessage << "`" << std::endl;
}

void main()
{
	TEXTURE_MANAGER.LoadTextures();

	SCENE_MANAGER.AddScene(AUTHENTICATION, new AuthenticateScene()); 
	SCENE_MANAGER.AddScene(ROOM, new RoomScene()); 
	SCENE_MANAGER.AddScene(WAITING, new WaitingScene());
	SCENE_MANAGER.AddScene(GAMEPLAY, new GameplayScene());

	SCENE_MANAGER.SetCurrentScene(AUTHENTICATION);
	SCENE_MANAGER.GetCurrentScene()->OnEnter();

	// TCP
	sf::TcpSocket socket; 

	// Render SFML
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode({ WIDTH, HEIGHT }), "Client");

	if (socket.connect(SERVER_IP, SERVER_PORT) != sf::Socket::Status::Done && !testingGameplay)
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
			SCENE_MANAGER.GetCurrentScene()->Update(0.f);

			// DRAW
			SCENE_MANAGER.GetCurrentScene()->Render(*window);

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