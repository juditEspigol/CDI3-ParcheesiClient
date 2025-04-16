// Client!!!!

#include <SFML/Network.hpp>
#include <iostream>
#include <string>

#define SERVER_PORT 55000 // puertos abiertos del 55000 - 55050

const sf::IpAddress SERVER_IP = sf::IpAddress(79, 152, 211, 184);//sf::IpAddress(10, 40, 2, 183); // Loopback

enum packetType { LOGIN, RESGISTER };

void HandShake(sf::Packet _data)
{
	std::string receivedMessage; 
	_data >> receivedMessage; // Sacar el mensaje del packet

	std::cout << "Mensaje recivido del servidor: `" << receivedMessage << "`" << std::endl;
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
	std::cout << "Hola soy el Cliente!" << std::endl;

	sf::TcpSocket socket; 
	bool gameLoop = true; 

	if (socket.connect(SERVER_IP, SERVER_PORT) != sf::Socket::Status::Done)
	{
		std::cerr << "Error al connectar con el servidor." << std::endl; 
	}
	else
	{
		std::cout << "Connectado con el servidor" << std::endl;

		while (gameLoop)
		{
			//sf::sleep(sf::seconds(1));
			std::string message, username, password; 
			std::cout << "Inserta mensaje para el servidor, -1 para salir" << std::endl; 
			std::cin >> message; 
			username = "Judith"; 
			password = "Espigol";

			if (message == "-1")
			{
				std::cout << "Desconectado..." << std::endl; 
				gameLoop = false; 
			}
			else
			{
				sf::Packet packet; 
				packet << packetType::LOGIN << username << password;
				SendData(socket, packet);
			}
		}
	}

	// Leer un pakcet (switch) 
	/*sf::Packet packet; 

	if (socket.receive(packet) == sf::Socket::Status::Done)
	{
		packetType type; 
		packet >> type; 

		switch (type)
		{
		case HANDSHAKE:
			HandShake(packet); 
			break;
		case LOGIN:
			break;
		case MOVEMENT:
			break;
		default:
			break;
		}

		packet.clear(); 
	}
	else 
	{
		std::cout << "Error al recivir mensaje del servidor" << std::endl; 
	}*/

	socket.disconnect(); 

	std::cout << "Desconectado del servidor" << std::endl;
}