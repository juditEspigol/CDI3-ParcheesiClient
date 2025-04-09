// Client!!!!

//#include <SFML/Network.hpp>
//#include <iostream>
//#include <string>
//
//#define SERVER_PORT 55000
//
//const sf::IpAddress SERVER_IP = sf::IpAddress(10,40,2,183); // Loopback 127,0,0,1
//
//enum packetType { HANDSHAKE, LOGIN, MOVEMENT };
//
//void HandShake(sf::Packet _data)
//{
//	std::string receivedMessage; 
//	_data >> receivedMessage; // Sacar el mensaje del packet
//
//	std::cout << "Mensaje recivido del servidor: `" << receivedMessage << "`" << std::endl;
//}
//
//sf::Packet& operator>>(sf::Packet& _packet, packetType& _type) 
//{
//	int temp;
//	_packet >> temp;
//	_type = static_cast<packetType>(temp);
//
//	return _packet;
//};
//
//void main()
//{
//	sf::TcpSocket socketServer;
//	bool gameLoop = true;
//
//	if (socketServer.connect(SERVER_IP, SERVER_PORT) == sf::Socket::Status::Disconnected)
//	{
//		std::cerr << "Error al connectar" << std::endl;
//	}
//	else
//	{
//		std::cout << "Conectado con el server" << std::endl;
//		while (gameLoop)
//		{
//			//sf::sleep(sf::seconds(1));
//			std::string message;
//			std::cout << "Inserta mensaje para el servvidor, -1 para salir" << std::endl;
//			std::cin >> message;
//
//			if (message == "-1")
//			{
//				std::cout << "Disconnect... " << std::endl;
//				gameLoop = false;
//			}
//			else
//			{
//				sf::Packet packet;
//				packet << message;
//				if (socketServer.send(packet) != sf::Socket::Status::Done)
//				{
//					std::cerr << "Error al enviar el paquete al servidor" << std::endl;
//
//				}
//				else
//				{
//					std::cout << "Mensaje enviado: " << message << std::endl;
//				}
//			}
//		}
//
//		socketServer.disconnect();
//	}

	/*std::cout << "Hola soy el Cliente!" << std::endl;

	sf::TcpSocket socket; 

	if (socket.connect(SERVER_IP, SERVER_PORT) != sf::Socket::Status::Done)
	{
		std::cerr << "Error al connectar con el servidor." << std::endl; 
	}

	std::cout << "Connectado con el servidor" << std::endl; 

	sf::Packet packet; 

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

	//socket.disconnect(); 
//}