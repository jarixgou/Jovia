#include "Utils.h"

sf::Vector2f Utils::IsoToOrtho(const sf::Vector3f& _point, const sf::Vector2f& _tileSize)
{
	sf::Vector2f pos;
	pos.x = (_point.x / _tileSize.x + _point.y / _tileSize.y) / 2;
	pos.y = (_point.x / _tileSize.x - _point.y / _tileSize.y) / 2;

	return pos;
}

sf::Vector2f Utils::OrthoToIso(const sf::Vector3f& _point, const sf::Vector2f& _tileSize)
{
	sf::Vector2f pos;
	pos.x = (_point.x - _point.y) * _tileSize.x;
	pos.y = (_point.x + _point.y - _point.z) * _tileSize.y;
	
	return pos;
}

int Utils::CalculateDepth(const sf::Vector3f& _point)
{
	return static_cast<int>(_point.x + _point.y + 0.001f * _point.z);
}
