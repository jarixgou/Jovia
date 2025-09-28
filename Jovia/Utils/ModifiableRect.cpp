#include "ModifiableRect.h"

Utils::ModifiableRect::ModifiableRect(sf::Vector2f _pos, sf::Vector2f _size)
{
	this->m_rect.setPrimitiveType(sf::PrimitiveType::Lines);

	sf::Vertex a(sf::Vector2f(_pos.x, _pos.y));
	sf::Vertex b(sf::Vector2f(_pos.x + _size.x, _pos.y));
	sf::Vertex c(sf::Vector2f(_pos.x + _size.x, _pos.y + _size.y));
	sf::Vertex d(sf::Vector2f(_pos.x, _pos.y + _size.y));

	this->m_rect.append(a);
	this->m_rect.append(b);

	this->m_rect.append(b);
	this->m_rect.append(c);

	this->m_rect.append(c);
	this->m_rect.append(d);

	this->m_rect.append(d);
	this->m_rect.append(a);

	this->m_bounds.left = _pos.x;
	this->m_bounds.top = _pos.y;
	this->m_bounds.width = _size.x;
	this->m_bounds.height = _size.y;

	this->m_center.x = _pos.x + _size.x / 2;
	this->m_center.y = _pos.y + _size.y / 2;
}

void Utils::ModifiableRect::UpdateRect(sf::Vector2f _mousePos)
{
	const float epsilon = 1e-2f;

	for (size_t i = 0; i < this->m_rect.getVertexCount(); ++i)
	{
		this->m_rect[i].color = sf::Color::White;
	}

	for (size_t i = 0; i < this->m_rect.getVertexCount(); i += 2)
	{
		sf::Vector2f A = this->m_rect[i].position;
		sf::Vector2f B = this->m_rect[i + 1].position;
		sf::Vector2f AB = B - A;
		sf::Vector2f AM = _mousePos - A;

		float cross = AB.x * AM.y - AB.y * AM.x;
		if (std::abs(cross) < epsilon)
		{
			float dot = AM.x * AB.x + AM.y * AB.y;
			float lenSq = AB.x * AB.x + AB.y * AB.y;
			if (dot >= 0 && dot <= lenSq)
			{
				this->m_rect[i].color = sf::Color::Red;
				this->m_rect[i + 1].color = sf::Color::Red;
			}
		}
	}
}

sf::FloatRect Utils::ModifiableRect::GetBounds() const
{
	return this->m_bounds;
}

sf::Vector2f Utils::ModifiableRect::GetCenter() const
{
	return this->m_center;
}
