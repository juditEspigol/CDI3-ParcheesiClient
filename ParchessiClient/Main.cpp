#include "SceneManager.h"
#include "ClientManager.h"
#include "NetworkInterface.h"

const sf::IpAddress SERVER_IP = sf::IpAddress(10, 40, 2, 208); //sf::IpAddress(10, 40, 2, 183); // Loopback /// 79, 152, 211, 184

bool isTesting = false;

void main()
{
	// LOAD ASSETS, SOUNDS, FONTS...
	TEXTURE_MANAGER.LoadTextures();

	// CREATE SCENES
	SCENE_MANAGER.AddScene(AUTHENTICATION, new AuthenticateScene()); 
	SCENE_MANAGER.AddScene(ROOM, new RoomScene()); 
	SCENE_MANAGER.AddScene(WAITING, new WaitingScene());
	SCENE_MANAGER.AddScene(GAMEPLAY, new GameplayScene());
	SCENE_MANAGER.SetCurrentScene(AUTHENTICATION);
	SCENE_MANAGER.GetCurrentScene()->OnEnter();

	// Render SFML
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode({ WIDTH, HEIGHT }), "ParchessiClient");
	// TCP
	sf::TcpSocket socket; 
	NETWORK_MANAGER.Init();

	if (socket.connect(SERVER_IP, SERVER_PORT) != sf::Socket::Status::Done && !isTesting)
	{
		std::cerr << "Error connecting to server." << std::endl;
	}
	else
	{
		//std::cout << "Connected to server" << std::endl;
		while (window->isOpen())
		{
		
			SCENE_MANAGER.GetCurrentScene()->Render(*window);

			// LISTENER
			while (const std::optional event = window->pollEvent())
			{
				//std::cout << "Click o algo" << std::endl;
				SCENE_MANAGER.GetCurrentScene()->HandleEvent(*event, *window, socket);
			}

			SCENE_MANAGER.GetCurrentScene()->Render(*window);

			// UPDATE
			SCENE_MANAGER.GetCurrentScene()->Update(0.f, socket);



			// CHANGE SCENE
			if (SCENE_MANAGER.GetCurrentScene()->GetIsFinished())
			{
				//std::cout << "Change scene!" << std::endl;
				std::string nextScene = SCENE_MANAGER.GetCurrentScene()->OnExit();
				SCENE_MANAGER.SetCurrentScene(nextScene);
				SCENE_MANAGER.GetCurrentScene()->OnEnter();
			}
		}
	}
	socket.disconnect();
	//std::cout << "Disconected from server" << std::endl;

	delete window;
}