#include "Cell.h"


Cell::Cell(int id, sf::Vector2f position, bool _isHoriz, bool _isFinal)
{
	_id = id;
	_position = position;	
	sf::Vector2f newSize = sf::Vector2f(CELL_WIDTH, CELL_HEIGTH);
	_shape.setFillColor(sf::Color::Magenta);
	_shape.setSize(newSize);
	_shape.setPosition(position);
	_isFinalCell = _isFinal;
	_isHorizontal = _isHoriz;
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
		std::vector<sf::Vector2f> baseOffsets = {
			sf::Vector2f(-60, -60),
			sf::Vector2f(60, -60),
			sf::Vector2f(60, 60),
			sf::Vector2f(-60, 60)
		};

		_tokens.push_back(token);

		for (size_t i = 0; i < _tokens.size(); ++i)
		{
			if (i < baseOffsets.size())
			{
				_tokens[i]->SetPosition(_position + baseOffsets[i], _id);
			}
		}
		return;
	}
	else if (_isFinalCell)
	{
		std::vector<sf::Vector2f> baseOffsets = {};

		if (_isHorizontal)
		{
			baseOffsets =
			{
				sf::Vector2f(0, 0),
				sf::Vector2f(-25, 0),
				sf::Vector2f(25, 0),
				sf::Vector2f(0, 25)
			};		
		}
		else
		{
			baseOffsets =
			{
				sf::Vector2f(0, 0),
				sf::Vector2f(-25, 0),
				sf::Vector2f(0, 25),
				sf::Vector2f(0, -25)
			};
		}

		_tokens.push_back(token);

		for (size_t i = 0; i < _tokens.size(); ++i)
		{
			if (i < baseOffsets.size())
			{
				_tokens[i]->SetPosition(_position + baseOffsets[i], _id);
			}
		}

		if (_tokens.size() == 4)
		{
			std::cout << "Has ganado" << std::endl;
		}
	}
	else
	{
		if (_isHorizontal)
		{
			if (_tokens.size() == 0)
				token->SetPosition(_position + sf::Vector2f(-20, 0), _id);
			else
				token->SetPosition(_position + sf::Vector2f(20, 0), _id);
		}
		else
		{
			if (_tokens.size() == 0)
				token->SetPosition(_position + sf::Vector2f(0, -20), _id);
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

