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

	switch (currentState)
	{
	case GameDirector::GameState::WAITING_TURN:
		dice->OnLeftClick(mousePressed, socket);
		if (dice->IsSelected())
		{
			gameDirector->CalculateMovableTokens();
		}
		break;

	case GameDirector::GameState::DICE_ROLLED:
		gameDirector->SelectToken(mousePressed->position);
		break;

	case GameDirector::GameState::TURN_COMPLETE:
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

void GameplayScene::HandleEvent(const sf::Event& _event, sf::RenderWindow& _window, sf::TcpSocket& _socket)
{
	if (_event.is < sf::Event::Closed>())
	{
		_window.close();
		return;
	}

	std::cout << gameDirector->GetCurrentPlayer() << " --> " << CLIENT_MANAGER.GetSelfID() << std::endl;
	if (CLIENT_MANAGER.GetSelfID() != gameDirector->GetCurrentPlayer())
		return;

	std::cout << "Me toca" << std::endl;
	if (const sf::Event::KeyPressed* keyPressed = _event.getIf<sf::Event::KeyPressed>())
	{
		HandleKeyPress(keyPressed, _window); // teclado
	}
	if (const sf::Event::MouseButtonPressed* mousePressed = _event.getIf<sf::Event::MouseButtonPressed>())
	{
		if (mousePressed->button == sf::Mouse::Button::Left)
		{
			HandleMouseClick(mousePressed, _socket); // raton
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
	for (Client* client : CLIENT_MANAGER.GetClients())
	{
		sf::Packet packet;
		
		if (client->GetSocket()->receive(packet) == sf::Socket::Status::Done)
		{
			
			std::cerr << "Recived packet" << std::endl;
			PacketType type;
			packet >> type;
			
			switch (type)
			{
			case DICE_ROLL:
			{
				std::cerr << "Dice roll " << std::endl;
				int diceValue;
				packet >> diceValue;
				dice->ForceDiceValue(diceValue);
				gameDirector->CalculateMovableTokens();
				break;
			}
			case END_TURN:
			{
				std::cerr << "End turn " << std::endl;
				gameDirector->EndTurn();
				break;
			}
			case MOVE_TOKEN:
			{
				std::cerr << "Move token " << std::endl;
				// Actualizar posición del token
				int tokenId, newPosition;
				packet >> tokenId >> newPosition;
				break;
			}
			default:
				std::cerr << "Tipo de paquete desconocido" << std::endl;
				break;
			}

			packet.clear();
		}
	}
}
