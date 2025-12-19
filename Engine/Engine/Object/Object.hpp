#ifndef DRAWABLE__HPP
#define DRAWABLE__HPP
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>

namespace Engine
{
	/**
	 * @struct Object
	 * @brief Container for drawable visual representation and render states
	 *
	 * Encapsulates an SFML drawable object (sprite, shape, etc.) along with
	 * its associated render states (blend mode, transform, texture, shader).
	 */
	struct Object
	{
		sf::Drawable* shape;             ///< Pointer to the drawable visual element
		sf::RenderStates renderStates;   ///< Rendering configuration (blending, texture, shader)
	};
}

#endif