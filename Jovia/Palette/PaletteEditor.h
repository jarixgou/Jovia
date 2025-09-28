#pragma once

#include "../Common.h"

class PaletteEditor
{
private:
	class Palette
	{
		sf::Texture m_texture;
		sf::Sprite m_sprite;
	};

public:
	static void Update(sf::RenderWindow& _window, float _dt);
};
