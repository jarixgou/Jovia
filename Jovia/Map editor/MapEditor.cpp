#include "MapEditor.h"

Grid::Grid(sf::Vector2f _gridSize, sf::Vector2f _cellSize)
{
	this->gridSize = _gridSize;
	this->cellSize = _cellSize;

	this->cellList.resize(_gridSize.y);

	this->cellTexture.loadFromFile("Assets/Cell.png");
	this->cellSelectorTexture.loadFromFile("Assets/CellSelector.png");

	int cellId = 0;

	for (int y = 0; y < _gridSize.y; y++)
	{
		this->cellList[y].resize(_gridSize.x);

		for (int x = 0; x < _gridSize.x; x++)
		{
			Cell* cell = &this->cellList[y][x];
			cell->id = cellId;
			cell->position = { x, y };
		
			cell->sprite.setTexture(this->cellTexture);

			sf::Vector2f isoPos = Utils::OrthoToIso({ (float)x, (float)y, 0}, _cellSize * 0.5f);
			cell->sprite.setPosition(isoPos);

			cellId++;
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
			_window.draw(cell.sprite);
		}
	}
}