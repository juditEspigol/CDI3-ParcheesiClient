#include "Scene.h"

std::string Scene::OnExit()
{
	isFinished = false;
	return nextScene;
}

void Scene::HandleEvent(const sf::Event& _event, sf::RenderWindow& _window, sf::TcpSocket& _socket)
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
		case sf::Keyboard::Key::Enter:
			for (Button* button : buttons)
			{
				button->OnEnter(keyPressed, _socket);
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
		default:
			break;
		}
	}
	if (const sf::Event::MouseButtonPressed* mousePressed = _event.getIf<sf::Event::MouseButtonPressed>())
	{
		switch (mousePressed->button)
		{
		case sf::Mouse::Button::Left:
			for (Button* button : buttons)
			{
				button->OnLeftClick(mousePressed, _socket);
			}
			break;
		default:
			break;
		}
	}
	if (const sf::Event::TextEntered* textEntered = _event.getIf<sf::Event::TextEntered>())
	{
		for (Button* button : buttons)
		{
			if (button->IsSelected())
			{
				button->GetText()->InsertChar(textEntered);
			}
		}
	}
}

void Scene::Update(float _dt)
{
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
