#include "DrawableObject.hpp"

#include <utility>

namespace Engine
{
	DrawableObject::DrawableObject(sf::Sprite _sprite, sf::RenderStates _states)
		: type(DrawableType::SPRITE), states(_states), sprite(std::move(_sprite))
	{

	}

	DrawableObject::DrawableObject(sf::RectangleShape _rectangle, sf::RenderStates _states)
		: type(DrawableType::RECTANGLE), states(_states), rectangle(std::move(_rectangle))
	{

	}

	DrawableObject::DrawableObject(sf::CircleShape _circle, sf::RenderStates _states)
		: type(DrawableType::CIRCLE), states(_states), circle(std::move(_circle))
	{

	}

	DrawableObject::DrawableObject(sf::VertexArray _shape, sf::RenderStates _states)
		: type(DrawableType::SHAPE), states(_states), shape(std::move(_shape))
	{

	}

	DrawableObject::DrawableObject(const DrawableObject& _other) noexcept
	{
		type = _other.type;
		switch (type)
		{
		case DrawableType::SPRITE:
			sprite = _other.sprite;
			break;
		case DrawableType::RECTANGLE:
			rectangle = _other.rectangle;
			break;
		case DrawableType::CIRCLE:
			circle = _other.circle;
			break;
		case DrawableType::SHAPE:
			shape = _other.shape;
			break;
		}
	}

	DrawableObject& DrawableObject::operator=(const DrawableObject& _other) noexcept
	{
		if (this != &_other)
		{
			type = _other.type;
			switch (type)
			{
			case DrawableType::SPRITE:
				sprite = _other.sprite;
				break;
			case DrawableType::RECTANGLE:
				rectangle = _other.rectangle;
				break;
			case DrawableType::CIRCLE:
				circle = _other.circle;
				break;
			case DrawableType::SHAPE:
				shape = _other.shape;
				break;
			}
		}
		return *this;
	}
}