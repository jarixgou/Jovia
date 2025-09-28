#include "MapEditor.h"

Grid::Grid(sf::Vector2f _gridSize, sf::Vector2f _cellSize)
{
	this->gridSize = _gridSize;
	this->cellSize = _cellSize;

	this->cellList.resize(_gridSize.y);

	for (int y = 0; y < _gridSize.y; y++)
	{
		this->cellList[y].resize(_gridSize.x);

		for (int x = 0; x < _gridSize.x; x++)
		{
			Cell* cell = &this->cellList[y][x];
			cell->id = x + y;
			cell->position = { x, y };

			cell->rectangle.setSize(_cellSize);
			cell->rectangle.setOutlineThickness(3);
			cell->rectangle.setOutlineColor(sf::Color::Red);
			cell->rectangle.setPosition(_cellSize.x * x, _cellSize.y * y);
		}
	}
}

Grid::~Grid()
{

}

void Grid::Draw(sf::RenderWindow& _window)
{
	for (std::vector<Cell>& cellList : this->cellList)
	{
		for (Cell& cell : cellList)
		{
			_window.draw(cell.rectangle);
		}
	}
}