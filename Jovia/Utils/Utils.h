#pragma once
#include "../Common.h"

namespace Utils
{
	sf::Vector2f IsoToOrtho(const sf::Vector3f& _point, const sf::Vector2f& _tileSize);
	sf::Vector2f OrthoToIso(const sf::Vector3f& _point, const sf::Vector2f& _tileSize);

	int CalculateDepth(const sf::Vector3f& _point);
}
