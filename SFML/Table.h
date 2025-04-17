#pragma once
#include "unordered_map"
#include "GameConfig.h"
#include "Cell.h"
#include "Token.h"
#include <nlohmann/json.hpp>

#define TABLE Table::Instance()
#define TOTAL_CELLS 67

class Table
{
private:
	Table() = default;
	Table(const Table&) = delete;
	Table& operator =(const Table&) = delete;

	std::unordered_map<int, Cell*> _cells;
	std::vector<Token*> _tokens;

public:
	void Init();
	void Draw(sf::RenderWindow& window);
	void Update();
	Cell* GetCell(int id);

	inline std::vector<Token*> GetTokens() { return _tokens; }

	inline static Table& Instance()
	{
		static Table singleton;
		return singleton;
	}
};