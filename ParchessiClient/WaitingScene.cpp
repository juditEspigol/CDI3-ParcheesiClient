#include "WaitingScene.h"
#include "GlobalValues.h"

WaitingScene::WaitingScene()
{
	isFinished = false;
	nextScene = GAMEPLAY;

	TextFill* waitingText = new TextFill("WAITING PLAYERS TO JOIN...", sf::Vector2f(1280 / 2 - 120, 720 / 2));

	texts.push_back(waitingText);
}

void WaitingScene::OnEnter()
{
	code = CODE;
	TextFill* codeText = new TextFill(CODE, sf::Vector2f(1280 / 2 - 25, 720 / 2 - 100));
	codeText->SetColor(sf::Color::White);
	texts.push_back(codeText);
}

void WaitingScene::Render(sf::RenderWindow& _window)
{
	_window.clear(sf::Color::Yellow);

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
