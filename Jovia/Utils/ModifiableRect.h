#pragma once

#include "../Common.h"

namespace Utils
{
	class ModifiableRect
	{
	private:
		sf::VertexArray rect;
		sf::FloatRect bounds;
		sf::Vector2f center;
	public:
		ModifiableRect(sf::Vector2f _pos, sf::Vector2f _size);

		void UpdateRect(sf::Vector2f _mousePos);

		sf::Vector2f GetBounds();
		sf::Vector2f GetCenter();
	};
}
