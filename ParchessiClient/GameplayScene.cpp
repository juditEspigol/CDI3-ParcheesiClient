#include "GameplayScene.h"

GameplayScene::GameplayScene()
{
	isFinished = false;
	nextScene = WAITING;

	table = new Table();
	dice = new Dice();
	buttons.push_back(dice);

	gameDirector = new GameDirector(*table, dice);

	tableSprite = new sf::Sprite(*TEXTURE_MANAGER.LoadTexture(TABLE_TEXTURE));

	gameDirector->StartGame();
}

GameplayScene::~GameplayScene()
{
	delete gameDirector;
}

void GameplayScene::OnEnter()
{
}

void GameplayScene::HandleEvent(const sf::Event& _event, sf::RenderWindow& _window, sf::TcpSocket& _socket)
{
	Scene::HandleEvent(_event, _window, _socket);

	if (_event.is < sf::Event::Closed>()) {
		_window.close();
	}
	if (const sf::Event::KeyPressed* keyPressed = _event.getIf<sf::Event::KeyPressed>()) {
		switch (keyPressed->code) {
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
	if (const sf::Event::MouseButtonPressed* mousePressed = _event.getIf<sf::Event::MouseButtonPressed>()) {
		if (mousePressed->button == sf::Mouse::Button::Left)
		{

			if (gameDirector->GetCurrentState() == GameDirector::GameState::WAITING_TURN)
			{
				dice->OnLeftClick(mousePressed, _socket);

				std::cout << "Current Game State: " << "Wating Turn" << std::endl;

				if (dice->HasBeenPressed())
				{
					gameDirector->CalculateMovableTokens();
				}
				return;
			}
			if (gameDirector->GetCurrentState() == GameDirector::GameState::DICE_ROLLED)
			{
				std::cout << "Current Game State: " << "Dice Rolled" << std::endl;

				gameDirector->SelectToken(mousePressed->position);
				return;
			}
		}
	}
}

void GameplayScene::Render(sf::RenderWindow& _window)
{
	_window.clear();

	// Dibujar mesa
	_window.draw(*tableSprite);
	
	dice->Render(_window);

	// Dibujar dado
	_window.draw(dice->GetTurnIndicator(gameDirector->GetCurrentPlayer(), WIDTH, HEIGHT));
	_window.draw(dice->GetDiceText());

	table->Draw(_window);
	_window.display();
}