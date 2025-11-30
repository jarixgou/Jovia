#include "DrawableObject.hpp"

namespace Engine
{
	DrawableObject::DrawableObject()
	{
		type = DrawableType::SPRITE;
		states = sf::RenderStates();
		new (&members.sprite) sf::Sprite();
	}

	DrawableObject::DrawableObject(sf::Sprite _sprite, sf::RenderStates _states)
	{
		type = DrawableType::SPRITE;
		states = _states;
		new (&members.sprite) sf::Sprite(std::move(_sprite));
	}

	DrawableObject::DrawableObject(sf::RectangleShape _rectangle, sf::RenderStates _states)
	{
		type = DrawableType::RECTANGLE;
		states = _states;
		new (&members.rectangle) sf::RectangleShape(std::move(_rectangle));
	}

	DrawableObject::DrawableObject(sf::CircleShape _circle, sf::RenderStates _states)
	{
		type = DrawableType::CIRCLE;
		states = _states;
		new (&members.circle) sf::CircleShape(std::move(_circle));
	}

	DrawableObject::DrawableObject(sf::VertexArray _shape, sf::RenderStates _states)
	{
		type = DrawableType::SHAPE;
		states = _states;
		new (&members.shape) sf::VertexArray(std::move(_shape));
	}

	DrawableObject::DrawableObject(const DrawableObject& _other) noexcept
	{
		type = _other.type;
		states = _other.states;
		switch (type)
		{
		case DrawableType::SPRITE:
			new (&members.sprite) sf::Sprite(_other.members.sprite);
			break;
		case DrawableType::RECTANGLE:
			new (&members.rectangle) sf::RectangleShape(_other.members.rectangle);
			break;
		case DrawableType::CIRCLE:
			new (&members.circle) sf::CircleShape(_other.members.circle);
			break;
		case DrawableType::SHAPE:
			new (&members.shape) sf::VertexArray(_other.members.shape);
			break;
		}
	}

	DrawableObject& DrawableObject::operator=(const DrawableObject& _other) noexcept
	{
		if (this == &_other) return *this;

		// If same active type, assign; otherwise destroy current and construct new
		if (type == _other.type)
		{
			switch (type)
			{
			case DrawableType::SPRITE:
				members.sprite = _other.members.sprite;
				break;
			case DrawableType::RECTANGLE:
				members.rectangle = _other.members.rectangle;
				break;
			case DrawableType::CIRCLE:
				members.circle = _other.members.circle;
				break;
			case DrawableType::SHAPE:
				members.shape = _other.members.shape;
				break;
			}
			states = _other.states;
			return *this;
		}

		// Destroy current
		switch (type)
		{
		case DrawableType::SPRITE:
			members.sprite.~Sprite();
			break;
		case DrawableType::RECTANGLE:
			members.rectangle.~RectangleShape();
			break;
		case DrawableType::CIRCLE:
			members.circle.~CircleShape();
			break;
		case DrawableType::SHAPE:
			members.shape.~VertexArray();
			break;
		}

		// Construct new
		type = _other.type;
		states = _other.states;
		switch (type)
		{
		case DrawableType::SPRITE:
			new (&members.sprite) sf::Sprite(_other.members.sprite);
			break;
		case DrawableType::RECTANGLE:
			new (&members.rectangle) sf::RectangleShape(_other.members.rectangle);
			break;
		case DrawableType::CIRCLE:
			new (&members.circle) sf::CircleShape(_other.members.circle);
			break;
		case DrawableType::SHAPE:
			new (&members.shape) sf::VertexArray(_other.members.shape);
			break;
		}

		return *this;
	}

	DrawableObject::~DrawableObject()
	{
		switch (type)
		{
		case DrawableType::SPRITE:
			members.sprite.~Sprite();
			break;
		case DrawableType::RECTANGLE:
			members.rectangle.~RectangleShape();
			break;
		case DrawableType::CIRCLE:
			members.circle.~CircleShape();
			break;
		case DrawableType::SHAPE:
			members.shape.~VertexArray();
			break;
		}
	}
}