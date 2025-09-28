#pragma once

#include "../Common.h"

namespace Utils
{
	class ModifiableRect
	{
	public:
		ModifiableRect(sf::Vector2f _pos, sf::Vector2f _size);

		sf::VertexArray rect;
		sf::FloatRect bounds;
		sf::Vector2f center;

	};
}
