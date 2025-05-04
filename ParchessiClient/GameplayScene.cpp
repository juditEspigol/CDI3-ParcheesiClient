#include "GameplayScene.h"

void GameplayScene::HandleKeyPress(const sf::Event::KeyPressed* keyPressed, sf::RenderWindow& window)
{
	switch (keyPressed->code)
	{
	case sf::Keyboard::Key::Escape:
		window.close();
		break;
	case sf::Keyboard::Key::Num1:
		dice->ForceDiceValue(1);
		break;
	case sf::Keyboard::Key::Num2:
		dice->ForceDiceValue(2);
		break;
	case sf::Keyboard::Key::Num3:
		dice->ForceDiceValue(3);
		break;
	case sf::Keyboard::Key::Num4:
		dice->ForceDiceValue(4);
		break;
	case sf::Keyboard::Key::Num5:
		dice->ForceDiceValue(5);
		break;
	case sf::Keyboard::Key::Num6:	
		dice->ForceDiceValue(6);
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
	Scene::HandleEvent(_event, _window, _socket);

	if (_event.is < sf::Event::Closed>()) {
		_window.close();
		return;
	}
	// Manejar eventos de teclado
	if (const sf::Event::KeyPressed* keyPressed = _event.getIf<sf::Event::KeyPressed>())
	{
		HandleKeyPress(keyPressed, _window);
	}

	// Manejar eventos de mouse
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

	// Draw Table
	_window.draw(*tableSprite);

	// Draw Dice
	_window.draw(dice->GetTurnIndicator(gameDirector->GetCurrentPlayer(), WIDTH, HEIGHT));
	_window.draw(dice->GetDiceText());

	// Draw End Turn
	_window.draw(endTurnButton->GetEndButton(WIDTH, HEIGHT));

	table->Draw(_window);


	_window.display();
}