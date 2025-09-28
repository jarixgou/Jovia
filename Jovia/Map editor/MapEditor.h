#pragma once

#include "../Common.h"

class Cell
{
public : 
	sf::Vector2i position;
	int id;
	sf::RectangleShape rectangle;
};

class Grid
{
public :
	Grid(sf::Vector2f _gridSize, sf::Vector2f _cellSize);
	~Grid();

	sf::Vector2f gridSize;
	sf::Vector2f cellSize;
	std::vector<std::vector<Cell>> cellList;

	void Draw(sf::RenderWindow& _window);
};

