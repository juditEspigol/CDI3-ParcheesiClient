#include "ButtonPacketSender.h"

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

ButtonPacketSender::ButtonPacketSender(PacketType _packetType, std::vector<ButtonTextUpdater*> _buttonTexts, sf::Vector2f _position)
 : Button(_position), packetType(_packetType), buttonTexts(_buttonTexts)
{
	sf::Vector2f positionText(_position.x + 10, _position.y + 5);
	switch (_packetType)
	{
	case LOGIN:
		text = new TextFill("LOG IN", positionText);
		break;
	case REGISTER:
		text = new TextFill("REGISTER", positionText);
		break;
	case JOIN_ROOM:
		text = new TextFill("CODE ROOM", positionText);
		break;
	case CREATE_ROOM:
		text = new TextFill("CREATE ROOM", positionText);
		break;
	default:
		break;
	}
	sprite->setTexture(*textureSelected);
	text->SetBoldFont();
	text->SetColor(sf::Color::Black);
}

ButtonPacketSender::~ButtonPacketSender()
{
}

void ButtonPacketSender::HandleEvent(const sf::Event& _event, sf::RenderWindow& /*_window*/, sf::TcpSocket& _socket)
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

				sf::Packet tempPacket;
				tempPacket << packetType;

				for (ButtonTextUpdater* button : buttonTexts)
				{
					if (button->GetTextContent() == "") 
					{
						std::cerr << "Invalid message!" << std::endl;
						return;
					}
					std::cout << button->GetTextContent() << std::endl;
					tempPacket << button->GetTextContent();
				}
				pressed = true;
				
				SendData(_socket, tempPacket);
			}
		}
		break;
		default:
			break;
		}
	}
}
