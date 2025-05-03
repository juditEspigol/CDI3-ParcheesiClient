#include "WaitingScene.h"

WaitingScene::WaitingScene()
{
	isFinished = false;
	nextScene = GAMEPLAY;

	TextFill* waitingText = new TextFill("WAITING PLAYERS TO JOIN...", sf::Vector2f(WIDTH / 2 - 120, HEIGHT / 2));

	texts.push_back(waitingText);
}

void WaitingScene::OnEnter()
{
	code = CODE;
	TextFill* codeText = new TextFill(CODE, sf::Vector2f(WIDTH / 2 - 25, HEIGHT / 2 - 100));
	codeText->SetColor(sf::Color::Yellow);

	texts.push_back(codeText);
}

void WaitingScene::Render(sf::RenderWindow& _window)
{
	_window.clear(sf::Color::Black);

	for (TextFill* text : texts)
	{
		text->Render(_window);
	}

	_window.display();
}

void WaitingScene::HandleEvent(const sf::Event& _event, sf::RenderWindow& _window, sf::TcpSocket& _socket)
{
	Scene::HandleEvent(_event, _window, _socket);
}
