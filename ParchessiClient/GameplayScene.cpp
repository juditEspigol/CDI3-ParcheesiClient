#include "GameplayScene.h"

GameplayScene::GameplayScene()
{
	isFinished = false;
	waitingPacket = false;
	nextScene = WAITING;

	table = new Table();
	gameDirector = new GameDirector(*table);

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
	/*Scene::HandleEvent(_event, _window, _socket);

	if (_event.is < sf::Event::Closed>()) {
		_window.close();
	}
	if (const sf::Event::KeyPressed* keyPressed = _event.getIf<sf::Event::KeyPressed>()) {
		switch (keyPressed->code) {
		case sf::Keyboard::Key::Escape:
			_window.close();
			break;
		case sf::Keyboard::Key::Num1:
			gameDirector->ForceDiceValue(1);
			break;
		case sf::Keyboard::Key::Num2:
			gameDirector->ForceDiceValue(2);
			break;
		case sf::Keyboard::Key::Num3:
			gameDirector->ForceDiceValue(3);
			break;
		case sf::Keyboard::Key::Num4:
			gameDirector->ForceDiceValue(4);
			break;
		case sf::Keyboard::Key::Num5:
			gameDirector->ForceDiceValue(5);
			break;
		case sf::Keyboard::Key::Num6:
			gameDirector->ForceDiceValue(6);
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
				std::cout << "Current Game State: " << "Wating Turn" << std::endl;

				gameDirector->RollDice();
				return;
			}
			if (gameDirector->GetCurrentState() == GameDirector::GameState::DICE_ROLLED)
			{
				std::cout << "Current Game State: " << "Dice Rolled" << std::endl;

				gameDirector->SelectToken(mousePressed->position);
				return;
			}
		}
	}*/
}

void GameplayScene::Render(sf::RenderWindow& _window)
{
	_window.clear();

	_window.draw(*tableSprite);
	_window.draw(gameDirector->GetTurnIndicator(WIDTH, HEIGHT));
	_window.draw(gameDirector->GetDiceText());
	table->Draw(_window);

	_window.display();
}