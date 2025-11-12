#include "DrawableObject.hpp"

namespace Engine
{
	DrawableObject::DrawableObject(sf::Sprite _sprite, sf::RenderStates _states)
		: type(DrawableType::SPRITE), sprite(_sprite), states(_states)
	{

	}

	DrawableObject::DrawableObject(sf::RectangleShape _rectangle, sf::RenderStates _states)
		: type(DrawableType::RECTANGLE), rectangle(_rectangle), states(_states)
	{

	}

	DrawableObject::DrawableObject(sf::CircleShape _circle, sf::RenderStates _states)
		: type(DrawableType::CIRCLE), circle(_circle), states(_states)
	{

	}

	DrawableObject::DrawableObject(sf::VertexArray _shape, sf::RenderStates _states)
		: type(DrawableType::SHAPE), shape(_shape), states(_states)
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