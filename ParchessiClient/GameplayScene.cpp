#include "GameplayScene.h"
#include "ClientManager.h"


void GameplayScene::HandleKeyPress(const sf::Event::KeyPressed* keyPressed, sf::RenderWindow& window)
{
	
	switch (keyPressed->code)
	{
		case sf::Keyboard::Key::Escape:
			std::cout << "Disconected..." << std::endl;
			window.close();
			break;
		case sf::Keyboard::Key::Enter:
		{
			std::cout << "Enter" << std::endl;

			// ENVIAR PACKETE SIEMPRE PRIMERO UN PACKETE TIPE
			sf::Packet packet;
			std::string content = "Hola que tal";
			packet << content;
			for (auto client : CLIENT_MANAGER.GetClients())
			{
				NETWORK_MANAGER.SendData(*client->GetSocket(), packet);
				std::cout << "Sent this content: " << content << std::endl;
			}
		}
		break;
		case sf::Keyboard::Key::Backspace:
			for (Button* button : buttons)
			{
				if (button->IsSelected())
				{
					button->GetText()->RemoveChar();
				}
			}
			break;
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

	PrintCurrentState(currentState);

	Token* movedToken;
	
	switch (currentState)
	{
	case GameDirector::GameState::WAITING_TURN:
		dice->OnLeftClick(mousePressed, socket);
		if (dice->IsSelected())
		{
			//SendDicePacket();
			gameDirector->CalculateMovableTokens();
		}
		break;

	case GameDirector::GameState::DICE_ROLLED:
		gameDirector->SelectToken(mousePressed->position);
		movedToken = gameDirector->GetSelectedToken();

		if (movedToken) {
			//SendTokenPacket(movedToken);
			gameDirector->SetState(GameDirector::GameState::TURN_COMPLETE);
		}
		break;

	case GameDirector::GameState::TURN_COMPLETE:
		endTurnButton->OnLeftClick(mousePressed, socket);
		if (endTurnButton->IsSelected())
		{
			//SendEndTurn();
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

	tableSprite = new sf::Sprite(*TEXTURE_MANAGER.LoadTexture(TABLE_TEXTURE));

	table = new Table();
	gameDirector = new GameDirector(*table);

	endTurnButton = new EndTurnButton(gameDirector);
	dice = new Dice(gameDirector);

	gameDirector->SetEndTurn(endTurnButton);
	gameDirector->SetDice(dice);

	buttons.push_back(endTurnButton);
	buttons.push_back(dice);

	gameDirector->StartGame();
}

GameplayScene::~GameplayScene()
{
	delete gameDirector;
	delete table;
	delete dice;
	delete endTurnButton;
	delete tableSprite;

	buttons.clear();
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
	dice->ForceDiceValue(diceValue);
	gameDirector->CalculateMovableTokens();

}

void GameplayScene::OnReceiveEndTurn()
{
	std::cerr << "End turn " << std::endl;
	gameDirector->EndTurn();
	bucles++;
}

void GameplayScene::OnReceiveMoveToken(int tokenID, int diceValue)
{
	for (Token* token : table->GetTokens())
	{
		if (token->GetTokenId() == tokenID)
		{
			token->Move(diceValue);
			table->UpdatePositions(diceValue);
			break;
		}
	}
}

void GameplayScene::SendDicePacket()
{
	sf::Packet packet;
	std::cout << "Sending Packet Dice: " << dice->GetDiceValue() << std::endl;

	for (Client* client : CLIENT_MANAGER.GetClients())
	{
		packet << DICE_ROLL << dice->GetDiceValue();
		NETWORK_MANAGER.SendData(*client->GetSocket(), packet);
	}
}

void GameplayScene::SendTokenPacket(Token* token)
{
	sf::Packet movePacket;
	std::cout << "Send Packet: Moved Tocken: ID:" << token->GetTokenId()
		<< " Dice Value:" << dice->GetDiceValue() << std::endl;
	for (Client* client : CLIENT_MANAGER.GetClients())
	{
		movePacket << MOVE_TOKEN << token->GetTokenId() << dice->GetDiceValue();
		NETWORK_MANAGER.SendData(*client->GetSocket(), movePacket);
	}
}

void GameplayScene::SendEndTurn()
{
	sf::Packet packet;
	for (Client* client : CLIENT_MANAGER.GetClients())
	{
		packet << PacketType::END_TURN;
		NETWORK_MANAGER.SendData(*client->GetSocket(), packet);
	}
}

void GameplayScene::HandleEvent(const sf::Event& _event, sf::RenderWindow& _window, sf::TcpSocket& _socket)
{
	//Scene::HandleEvent(_event, _window, _socket);
	if (_event.is < sf::Event::Closed>())
	{
		_window.close();
		return;
	}
	//std::cout << "My id: " << gameDirector->GetCurrentPlayer() << "--->" << CLIENT_MANAGER.GetSelfID() <<  std::endl;
	if (CLIENT_MANAGER.GetSelfID() != gameDirector->GetCurrentPlayer())
		return;

	if (const sf::Event::KeyPressed* keyPressed = _event.getIf<sf::Event::KeyPressed>())
	{
		HandleKeyPress(keyPressed, _window);
	}
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
			
		}
		
	}
	
}