#ifndef TRANSFORM__HPP
#define TRANSFORM__HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

namespace Engine
{
	/**
	 * @struct Transform
	 * @brief Stores spatial transformation data for game objects
	 *
	 * Contains all information needed to position, scale, and rotate
	 * objects in 3D space with 2D scale support.
	 */
	struct Transform
	{
		sf::Vector3f position; ///< Position in 3D world space (x, y, z)
		sf::Vector3f size;     ///< Object dimensions (width, height, depth)
		sf::Vector2f scale;    ///< Scale factors (x, y)
		float angle;           ///< Rotation angle in degrees
	};
}

#endif