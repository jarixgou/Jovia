#pragma once

#include "../Common.h"
#include "../Utils/ModifiableRect.h"

class PaletteInterface
{
private:
	class Palette
	{
	public:
		std::string m_name;

		sf::Texture m_texture;
		sf::Sprite m_sprite;
		std::vector<Utils::ModifiableRect> m_rectsList;

		bool m_selected;
	};

	std::unique_ptr<sf::RenderTexture> m_paletteTexture;

	std::vector<Palette> m_palettesList;
public:
	void Load(void);
	void Update(sf::RenderWindow& _window, float _dt);
};
