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
		//Si es una base tiene 4 posiciones distintas en funcion de la cantidad de fichas que hay
		std::vector<sf::Vector2f> baseOffsets = {
			sf::Vector2f(-60, -60),
			sf::Vector2f(60, -60),
			sf::Vector2f(60, 60),
			sf::Vector2f(-60, 60)
		};

		_tokens.push_back(token);

		//Recolocamos las fichas de la celda en función del size
		for (size_t i = 0; i < _tokens.size(); ++i)
		{
			if (i < baseOffsets.size())
			{
				_tokens[i]->SetPosition(_position + baseOffsets[i], _id);
			}
		}
		return;
	}

	else
	{
		//Si son horizontales las dibujamos a la izquierda o derecha
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

		//Si son verticales las dibujamos abajo o arriba
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

	//Lo metemos en el vector de tokens de la celda
	_tokens.push_back(token);	
}

void Cell::RemoveToken(Token* token)
{
	//Buscamos y sacamos el token del vector
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

