#pragma once
#include "GameConfig.h"
#include "Cell.h"
#include <nlohmann/json.hpp>

#define TOTAL_CELLS 67

class Table
{
private:
	std::vector<Cell*> _cells;
	sf::RenderWindow* _window;
public:
	Table(sf::RenderWindow& window);
	void Draw();
	Cell* GetCell(int id);
};