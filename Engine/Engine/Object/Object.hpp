#ifndef DRAWABLE__HPP
#define DRAWABLE__HPP
#include <SFML/Graphics/Drawable.hpp>

namespace Engine
{
	struct Object
	{
		sf::Drawable* shape;
		sf::RenderStates renderStates;
	};
}

#endif