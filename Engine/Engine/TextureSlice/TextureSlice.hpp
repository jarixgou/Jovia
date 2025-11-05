#ifndef TEXTURE_SLICE_HPP
#define TEXTURE_SLICE_HPP

#include <string>
#include <vector>

#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/Texture.hpp"

namespace Engine
{
	struct TextureSliced
	{
		int id;
		sf::IntRect rect;
	};

	std::vector<TextureSliced> SliceTexture(const sf::Texture& _texture, sf::Vector2i _cellSize);
}

#endif