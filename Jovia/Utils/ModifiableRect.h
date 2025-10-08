#pragma once

#include "../Common.h"

namespace Utils
{
	class ModifiableRect
	{
	private:
		sf::VertexArray m_rect;
		sf::FloatRect m_bounds;
		sf::Vector2f m_center;
	public:
		ModifiableRect();
		ModifiableRect(sf::Vector2f _pos, sf::Vector2f _size);

		void UpdateRect(sf::Vector2f _mousePos);

		sf::FloatRect GetBounds() const;
		sf::Vector2f GetCenter() const;
		sf::VertexArray GetShape();
	};
}
