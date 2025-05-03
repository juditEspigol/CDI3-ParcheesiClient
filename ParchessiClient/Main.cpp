#include "SceneManager.h"
#include "ClientManager.h"

const sf::IpAddress SERVER_IP = sf::IpAddress(79, 152, 140, 103); //sf::IpAddress(10, 40, 2, 183); // Loopback /// 79, 152, 211, 184

// FOR TESTING
bool testingGameplay = false;

void main()
{
	TEXTURE_MANAGER.LoadTextures();

	// CREATE SCENES
	SCENE_MANAGER.AddScene(AUTHENTICATION, new AuthenticateScene()); 
	SCENE_MANAGER.AddScene(ROOM, new RoomScene()); 
	SCENE_MANAGER.AddScene(WAITING, new WaitingScene());
	SCENE_MANAGER.AddScene(GAMEPLAY, new GameplayScene());

	SCENE_MANAGER.SetCurrentScene(AUTHENTICATION);
	SCENE_MANAGER.GetCurrentScene()->OnEnter();

	// TCP
	sf::TcpSocket socket; 
	sf::SocketSelector selector;
	sf::TcpListener listener;
	if (listener.listen(LISTENER_PORT) != sf::Socket::Status::Done) // Comprbar puerto valido
	{
		std::cerr << "Cannot Listen the port.\nExiting execution with code -1." << std::endl;
		return;
	}
	selector.add(listener);

	// Render SFML
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode({ WIDTH, HEIGHT }), "ParchessiClient");

	if (socket.connect(SERVER_IP, SERVER_PORT) != sf::Socket::Status::Done && !testingGameplay)
	{
		std::cerr << "Error connecting to server." << std::endl;
	}
	else
	{
		std::cout << "Connected to server" << std::endl;

		std::cout << "My IPadress: " << sf::IpAddress::getPublicAddress().value() << ": " << socket.getLocalPort() << std::endl; 

		while (window->isOpen())
		{
			// LISTENER
			while (const std::optional event = window->pollEvent())
			{
				SCENE_MANAGER.GetCurrentScene()->HandleEvent(*event, *window, socket);
				
				// ESTEM TOTA LA ESTONA ESCOLTANT SI ALGU ES CONECTA
				if (!selector.isReady(listener))
				{
					for (Client* client : CLIENT_MANAGER.GetClients())
					{
						if (selector.isReady(*client->GetSocket()))
						{
							sf::Packet packet;
							if (client->GetSocket()->receive(packet) == sf::Socket::Status::Done)
							{
								// recieve packet
							}
							if (client->GetSocket()->receive(packet) == sf::Socket::Status::Disconnected)
							{
								// Stop game
							}
						}
					}
				}
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
	std::cout << "Disconected from server" << std::endl;

	delete window;
}