#include "Cell.h"


Cell::Cell(int id, sf::Vector2f position, bool _isHoriz)
{
	_id = id;
	_position = position;	
	sf::Vector2f newSize = sf::Vector2f(CELL_WIDTH, CELL_HEIGTH);
	_shape.setFillColor(sf::Color::Magenta);
	_shape.setSize(newSize);
	_shape.setPosition(position);
	_isHorizontal = _isHoriz;
}

void Cell::Draw(sf::RenderWindow& window)
{
	window.draw(_shape);
	window.display();
}

void Cell::AddToken(Token* token)
{
	for (Token* a : _tokens)
	{
		if (a == token)
		{
			return;
		}
	}

	if (_id >= 1000)
	{
		switch (_tokens.size())
		{
			case 0:
				token->SetPosition(_position + sf::Vector2f(-60, -60), _id);
			break;

			case 1:
				token->SetPosition(_position + sf::Vector2f(60, -60), _id);
			break;

			case 2:
				token->SetPosition(_position + sf::Vector2f(60, 60), _id);
			break;

			case 3:
				token->SetPosition(_position + sf::Vector2f(-60, 60), _id);
			break;
		}
	}

	else
	{
		if (_isHorizontal)
		{
			if (_tokens.size() == 0)
			{
				token->SetPosition(_position + sf::Vector2f(-20, 0), _id);
			}
			else
			{
				token->SetPosition(_position + sf::Vector2f(20, 0), _id);
			}
		}

		else
		{
			if (_tokens.size() == 0)
			{
				token->SetPosition(_position + sf::Vector2f(0, -20), _id);
			}
			else
				token->SetPosition(_position + sf::Vector2f(0, 20), _id);
		}
	}
	
	_tokens.push_back(token);
}

void Cell::RemoveToken(Token* token)
{
	for (size_t i = 0; i < _tokens.size(); ++i)
	{
		if (_tokens[i] == token)
		{
			_tokens.erase(_tokens.begin() + i);
			break;
		}
	}
}

std::vector<Token*> Cell::GetTokens()
{
	return _tokens;
}

