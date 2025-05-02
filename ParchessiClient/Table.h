#pragma once
#include "unordered_map"
#include "GameConfig.h"
#include "Cell.h"
#include "Token.h"
#include <nlohmann/json.hpp>

#define TOTAL_CELLS 67

class Table
{
private:

	std::unordered_map<int, Cell*> _cells;
	std::vector<Token*> _tokens;

public:
	Table();
	void Draw(sf::RenderWindow& window);
	void UpdatePositions(int newPos);
	Cell* GetCell(int id);

	std::vector<Token*> GetTokens() { return _tokens; }

	inline static Table& Instance()
	{
		static Table singleton;
		return singleton;
	}
};