#include "WaitingScene.h"
#include "ClientManager.h"

WaitingScene::WaitingScene()
{
	isFinished = false;
	waitingPacket = true;
	nextScene = GAMEPLAY;

	TextFill* waitingText = new TextFill("WAITING PLAYERS TO JOIN...", sf::Vector2f(WIDTH / 2 - 120, HEIGHT / 2));

	texts.push_back(waitingText);
}

void WaitingScene::OnEnter()
{
	code = CODE;
	TextFill* codeText = new TextFill(CODE, sf::Vector2f(WIDTH / 2 - 25, HEIGHT / 2 - 100));
	codeText->SetColor(sf::Color::Yellow);

	texts.push_back(codeText);
}

void WaitingScene::Render(sf::RenderWindow& _window)
{
	_window.clear(sf::Color::Black);

	for (TextFill* text : texts)
	{
		text->Render(_window);
	}

	_window.display();
}

void WaitingScene::Update(float _dt, sf::TcpSocket& _socket)
{
	if (CLIENT_MANAGER.GetSelfID() > 0)
	{
		if (NETWORK_MANAGER.GetConnect() <= 0)
		{
			NETWORK_MANAGER.CheckConnections();
			if (NETWORK_MANAGER.GetListen() <= 0)
			{
				isFinished = true;
				return;
			}
			return;
		}
	}

	sf::Packet tempPacket;
	if (_socket.receive(tempPacket) == sf::Socket::Status::Done)
	{
		PacketType type;
		tempPacket >> type;
		int validateAuthentication;

		// cear un nuevo packete que setee lo demas
		switch (type)
		{
		case SV_CONNECT_DATA:
		{
			int id, listen, connect;

			tempPacket >> id;
			tempPacket >> listen;
			tempPacket >> connect;

			CLIENT_MANAGER.SetSelfID(id);
			NETWORK_MANAGER.SetListen(listen);
			NETWORK_MANAGER.SetConnect(connect);

			std::cout << "Id: " << CLIENT_MANAGER.GetSelfID() << "-- Listen: " << NETWORK_MANAGER.GetListen() << "-- Connect: " << NETWORK_MANAGER.GetConnect() << std::endl;
			tempPacket.clear();
			return;
		}
		break;
		case SV_SOCKET:
		{
			// Read packet
			std::pair<sf::IpAddress, unsigned short> address(sf::IpAddress::Any, 0);
			tempPacket >> address;
			int port;
			tempPacket >> port;

			NETWORK_MANAGER.ConnectToSocket(address.first, port);
			//NETWORK_MANAGER.ConnectToSocket(address.first, address.second);

			tempPacket.clear();
			return;
		}
		break;
		default:
			break;
		}
	}
	tempPacket.clear();
}