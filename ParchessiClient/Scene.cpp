#include "Scene.h"

void Scene::GeneralEvents(const sf::Event& _event, sf::RenderWindow& _window, sf::TcpSocket& _socket)
{
	if (_event.is < sf::Event::Closed>())
	{
		_window.close();
	}
	if (const sf::Event::KeyPressed* keyPressed = _event.getIf<sf::Event::KeyPressed>())
	{
		switch (keyPressed->code)
		{
		case sf::Keyboard::Key::Escape:
			std::cout << "Disconected..." << std::endl;
			_window.close();
			break;
		default:
			break;
		}
	}
}

std::string Scene::OnExit()
{
	isFinished = false;
	return nextScene;
}

void Scene::HandleEvent(const sf::Event& _event, sf::RenderWindow& _window, sf::TcpSocket& _socket)
{
	GeneralEvents(_event, _window, _socket);

	for (Button* button : buttons)
	{
		button->HandleEvent(_event, _window, _socket);
	}
}

void Scene::Render(sf::RenderWindow& _window)
{
	_window.clear(sf::Color::Black);

	for (Button* button : buttons)
	{
		button->Render(_window);
	}

	_window.display();
}
