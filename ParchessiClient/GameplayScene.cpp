#include "GameplayScene.h"
#include "ClientManager.h"

void GameplayScene::HandleKeyPress(const sf::Event::KeyPressed* keyPressed, sf::RenderWindow& window)
{
	switch (keyPressed->code)
	{
		case sf::Keyboard::Key::Escape:
			// Cierra la ventana del juego
			std::cout << "Disconected..." << std::endl;
			window.close();
			break;
		case sf::Keyboard::Key::Enter:
		{
			// Envía un mensaje de prueba a todos los clientes conectados
			std::cout << "Enter" << std::endl;

			sf::Packet packet;
			std::string content = "Test Packet";
			packet << content;
			for (auto client : CLIENT_MANAGER.GetClients())
			{
				NETWORK_MANAGER.SendData(*client->GetSocket(), packet);
				std::cout << "Sent this content: " << content << std::endl;
			}
		}
		break;
		case sf::Keyboard::Key::Backspace:
			// Borra un carácter del botón seleccionado
			for (Button* button : buttons)
			{
				if (button->IsSelected())
				{
					button->GetText()->RemoveChar();
				}
			}
			break;
		// Forzar valor del dado con teclas del 1 al 6 y recalcular fichas que se pueden mover
		case sf::Keyboard::Key::Num1:
			dice->ForceDiceValue(1);
			gameDirector->SetState(GameDirector::GameState::WAITING_TURN);
			gameDirector->CalculateMovableTokens();
			break;
		case sf::Keyboard::Key::Num2:
			dice->ForceDiceValue(2);
			gameDirector->SetState(GameDirector::GameState::WAITING_TURN);
			gameDirector->CalculateMovableTokens();
			break;
		case sf::Keyboard::Key::Num3:
			dice->ForceDiceValue(3);
			gameDirector->SetState(GameDirector::GameState::WAITING_TURN);
			gameDirector->CalculateMovableTokens();
			break;
		case sf::Keyboard::Key::Num4:
			dice->ForceDiceValue(4);
			gameDirector->SetState(GameDirector::GameState::WAITING_TURN);
			gameDirector->CalculateMovableTokens();
			break;
		case sf::Keyboard::Key::Num5:
			dice->ForceDiceValue(5);
			gameDirector->SetState(GameDirector::GameState::WAITING_TURN);
			gameDirector->CalculateMovableTokens();
			break;
		case sf::Keyboard::Key::Num6:
			dice->ForceDiceValue(6);
			gameDirector->SetState(GameDirector::GameState::WAITING_TURN);
			gameDirector->CalculateMovableTokens();
			break;
	}
}

void GameplayScene::HandleMouseClick(const sf::Event::MouseButtonPressed* mousePressed, sf::TcpSocket& socket)
{
	GameDirector::GameState currentState = gameDirector->GetCurrentState();
	// Imprime el estado actual para debug
	PrintCurrentState(currentState);

	Token* movedToken;
	
	switch (currentState)
	{
	case GameDirector::GameState::WAITING_TURN:
		// Si estamos esperando turno, dejamos lanzar el dado
		dice->OnLeftClick(mousePressed, socket);
		if (dice->IsSelected())
		{
			// calcular fichas movibles tras lanzar el dado
			gameDirector->CalculateMovableTokens();
		}
		break;

	case GameDirector::GameState::DICE_ROLLED:
		// Si el dado ya fue lanzado, podemos seleccionar una ficha
		gameDirector->SelectToken(mousePressed, socket);
		movedToken = gameDirector->GetSelectedToken();

		if (movedToken) 
		{
			// Actualizamos la posición de la ficha seleccionada
			table->UpdatePositions(movedToken->Move(dice->GetDiceValue()));
			gameDirector->SetState(GameDirector::GameState::TURN_COMPLETE);
		}
		break;

	case GameDirector::GameState::TURN_COMPLETE:
		// Si ya se ha movido una ficha, permitimos terminar el turno
		endTurnButton->OnLeftClick(mousePressed, socket);
		if (endTurnButton->IsSelected())
		{
			gameDirector->EndTurn();
		}
		break;
	}
}

void GameplayScene::PrintCurrentState(GameDirector::GameState state)
{
	std::cout << "Current Game State: ";

	switch (state)
	{
	case GameDirector::GameState::WAITING_TURN:
		std::cout << "Waiting Turn";
		break;
	case GameDirector::GameState::DICE_ROLLED:
		std::cout << "Dice Rolled";
		break;
	case GameDirector::GameState::PIECE_SELECTED:
		std::cout << "Piece Selected";
		break;
	case GameDirector::GameState::TURN_COMPLETE:
		std::cout << "Turn Complete";
		break;
	default:
		std::cout << "Unknown";
		break;
	}

	std::cout << std::endl;
}
GameplayScene::GameplayScene()
{
	isFinished = false;
	waitingPacket = false;
	nextScene = WAITING;

	tableSprite = new sf::Sprite(*TEXTURE_MANAGER.LoadTexture(TABLE_TEXTURE)); // fondo del tablero

	table = new Table();                         // Carga las celdas desde el JSON
	gameDirector = new GameDirector(*table);     // Controlador principal del juego
	table->InitTokens(gameDirector);             // Coloca las fichas iniciales

	endTurnButton = new EndTurnButton(gameDirector);
	dice = new Dice(gameDirector);

	gameDirector->SetEndTurn(endTurnButton);
	gameDirector->SetDice(dice);

	buttons.push_back(endTurnButton);
	buttons.push_back(dice);

	gameDirector->StartGame(); // Empieza el primer turno
}

GameplayScene::~GameplayScene()
{
	delete gameDirector;
	delete table;
	delete dice;
	delete endTurnButton;
	delete tableSprite;

	buttons.clear(); // Limpieza de punteros a botones
}

void GameplayScene::OnEnter()
{
}

void GameplayScene::OnReceivePacket(sf::Packet packet)
{
	std::cerr << "Recibido paquete" << std::endl;
	PacketType type;
	packet >> type;

	Token* movedToken = gameDirector->GetSelectedToken();
	int newPos = gameDirector->GetNewTokenPosition();

	switch (type)
	{
	case DICE_ROLL:
	{
		int diceValue;
		packet >> diceValue;
		OnReceiveDiceRoll(diceValue);
		break;
	}
	case END_TURN:
	{
		OnReceiveEndTurn();
		break;
	}
	case MOVE_TOKEN: {
		int tokenId, newPosition;
		packet >> tokenId >> newPosition;

		OnReceiveMoveToken(tokenId, newPosition);
		break;
	}
	default:
		std::cerr << "Tipo de paquete desconocido" << std::endl;
		break;
	}

	packet.clear();
}

void GameplayScene::OnReceiveDiceRoll(int diceValue)
{
	std::cout << "Recived dice value packet with value: " << diceValue << std::endl;

	dice->ForceDiceValue(diceValue); // Forzar valor recibido
	gameDirector->SetState(GameDirector::GameState::TURN_COMPLETE); // Se salta directamente al final del turno
}

void GameplayScene::OnReceiveEndTurn()
{
	std::cerr << "End turn " << std::endl;
	gameDirector->EndTurn();
	bucles++; // contador de bucles
}

void GameplayScene::OnReceiveMoveToken(int tokenID, int diceValue)
{
	for (Token* token : table->GetTokens())
	{
		if (token->GetTokenId() == tokenID)
		{
			gameDirector->MoveTokenById(tokenID, diceValue); // mueve internamente el token
			table->UpdatePositions(diceValue);               // lo actualiza visualmente
			break;
		}
	}
}

void GameplayScene::HandleEvent(const sf::Event& _event, sf::RenderWindow& _window, sf::TcpSocket& _socket)
{
	if (_event.is < sf::Event::Closed>())
	{
		_window.close(); // Si se cierra la ventana
		return;
	}

	// Si no es tu turno, no puedes interactuar
	if (CLIENT_MANAGER.GetSelfID() != gameDirector->GetCurrentPlayer())
		return;
		
	// Tecla pulsada
	if (const sf::Event::KeyPressed* keyPressed = _event.getIf<sf::Event::KeyPressed>())
	{
		HandleKeyPress(keyPressed, _window);
	}
	// Clic izquierdo del ratón
	if (const sf::Event::MouseButtonPressed* mousePressed = _event.getIf<sf::Event::MouseButtonPressed>())
	{
		if (mousePressed->button == sf::Mouse::Button::Left)
		{
			HandleMouseClick(mousePressed, _socket);
		}
	}
}

void GameplayScene::Render(sf::RenderWindow& _window)
{
	_window.clear();

	_window.draw(*tableSprite);

	_window.draw(dice->GetTurnIndicator(gameDirector->GetCurrentPlayer(), WIDTH, HEIGHT));
	_window.draw(dice->GetDiceText());

	_window.draw(endTurnButton->GetEndButton(WIDTH, HEIGHT));
	_window.draw(endTurnButton->GetText());

	table->Draw(_window);

	_window.display();
}

void GameplayScene::Update(float _dt, sf::TcpSocket& _socket)
{
	
	if (CLIENT_MANAGER.GetSelfID() == gameDirector->GetCurrentPlayer())
		return;
	
	// Esperar hasta que al menos un socket tenga datos disponibles (sin bloquear)
	for (Client* client : CLIENT_MANAGER.GetClients())
	{
		sf::TcpSocket* socket = client->GetSocket();
		
		sf::Packet packet;
		if (socket->receive(packet) == sf::Socket::Status::Done)
		{
			OnReceivePacket(packet);
		}
	}
}