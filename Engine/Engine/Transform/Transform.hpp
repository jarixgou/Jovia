#ifndef TRANSFORM__HPP
#define TRANSFORM__HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

namespace Engine
{
	struct Transform
	{
		Transform();
		sf::Vector3f position;
		sf::Vector3f size;
		sf::Vector2f scale;
		float angle;
	};
}

#endif