#include "GameplayScene.h"

GameplayScene::GameplayScene()
{
	isFinished = false;
	nextScene = WAITING;

	tableSprite = new sf::Sprite(*TEXTURE_MANAGER.LoadTexture(TABLE_TEXTURE));

	table = new Table();
	gameDirector = new GameDirector(*table);

	dice = new Dice(gameDirector);
	endTurnButton = new EndTurnButton(gameDirector);

	gameDirector->SetDice(dice);
	gameDirector->SetEndTurn(endTurnButton);

	buttons.push_back(dice);
	buttons.push_back(endTurnButton);

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

	if (const sf::Event::KeyPressed* keyPressed = _event.getIf<sf::Event::KeyPressed>()) 
	{
		switch (keyPressed->code) 
		{
		case sf::Keyboard::Key::Escape:
			_window.close();
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
		default:
			break;
		}
	}
	if (const sf::Event::MouseButtonPressed* mousePressed = _event.getIf<sf::Event::MouseButtonPressed>()) 
	{
		if (mousePressed->button == sf::Mouse::Button::Left)
		{
			if (gameDirector->GetCurrentState() == GameDirector::GameState::WAITING_TURN)
			{
				std::cout << "Current Game State: " << "Wating Turn" << std::endl;

				dice->OnLeftClick(mousePressed, _socket);

				if (dice->IsSelected())
				{
					gameDirector->CalculateMovableTokens();
				}

				return;
			}
			if (gameDirector->GetCurrentState() == GameDirector::GameState::DICE_ROLLED)
			{
				std::cout << "Current Game State: " << "Dice Rolled" << std::endl;

				endTurnButton->OnLeftClick(mousePressed, _socket);
				gameDirector->SelectToken(mousePressed->position);

				return;
			}
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