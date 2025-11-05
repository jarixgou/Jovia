#include "TextureSlice.hpp"

std::vector<Engine::TextureSliced> Engine::SliceTexture(const sf::Texture& _texture, sf::Vector2i _cellSize)
{
	std::vector<TextureSliced> textureSliced;

	sf::Vector2u textureSize = _texture.getSize();

	sf::Image image = _texture.copyToImage();

	int id = 1;

	for (int y = 0; y < textureSize.y / _cellSize.y; ++y)
	{
		for (int x = 0; x < textureSize.x / _cellSize.x; ++x)
		{
			TextureSliced texture;
			texture.id = id;
			texture.rect = { x * _cellSize.x, y * _cellSize.y,
				_cellSize.x, _cellSize.y };

			if (texture.rect.top + texture.rect.height <= textureSize.y && 
				texture.rect.left + texture.rect.width <= textureSize.x)
			{
				bool found = false;
				for (int yI = texture.rect.top; yI <= texture.rect.top + texture.rect.height && !found; ++yI)
				{
					for (int xI = texture.rect.left; xI <= texture.rect.left + texture.rect.width && !found; ++xI)
					{
						if (image.getPixel(xI, yI).a > 0 && !found)
						{
							textureSliced.push_back(texture);
							id++;
							found = true;
						}
					}
				}
			}
		}
	}

	return textureSliced;
}