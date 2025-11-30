#ifndef DRAWABLE_OBJECT__HPP
#define DRAWABLE_OBJECT__HPP

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace Engine
{
	enum class DrawableType : uint8_t
	{
		SPRITE,
		RECTANGLE,
		CIRCLE,
		SHAPE
	};

	struct DrawableObject
	{
		DrawableType type;
		sf::RenderStates states;

		union Members
		{
			sf::Sprite sprite;
			sf::RectangleShape rectangle;
			sf::CircleShape circle;
			sf::VertexArray shape;

			Members() {}
			~Members() {}
		} members;

		DrawableObject();
		DrawableObject(sf::Sprite _sprite, sf::RenderStates _states);
		DrawableObject(sf::RectangleShape _rectangle, sf::RenderStates _states);
		DrawableObject(sf::CircleShape _circle, sf::RenderStates _states);
		DrawableObject(sf::VertexArray _shape, sf::RenderStates _states);

		DrawableObject(const DrawableObject& _other) noexcept;
		DrawableObject& operator=(const DrawableObject& _other) noexcept;
		~DrawableObject();
	};
}

#endif // !DRAWABLE_OBJECT__HPP