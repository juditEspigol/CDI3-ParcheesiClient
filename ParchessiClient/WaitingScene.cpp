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

void WaitingScene::HandleEvent(const sf::Event& _event, sf::RenderWindow& _window, sf::TcpSocket& _socket)
{
	sf::Packet tempPacket;
	if (_socket.receive(tempPacket) == sf::Socket::Status::Done)
	{
		PacketType type;
		tempPacket >> type;
		int validateAuthentication;

		switch (type)
		{
		case SV_SOCKET:
		{
			// Read packet
			std::pair<sf::IpAddress, unsigned short> address(sf::IpAddress::Any, 0);
			tempPacket >> address;

			sf::TcpSocket* socket = new sf::TcpSocket();
			std::cout << "Trying to connect with... " << address.first.toString() << " : " << address.second << "..." << std::endl;

			if (socket->connect(address.first, LISTENER_PORT) != sf::Socket::Status::Done)
			{
				std::cerr << "Error connecting to client: " << address.first.toString() << std::endl;
			}
			else
			{
				std::cout << "Connect to other client" << std::endl;
				isFinished = true;
				return;
			}
		}
		break;
		default:
			break;
		}
	}
	tempPacket.clear();
}
