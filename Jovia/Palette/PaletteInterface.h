#pragma once

#include "../Common.h"
#include <memory>
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
		std::vector<std::vector<Utils::ModifiableRect>> m_rectsList;
	};

	std::unique_ptr<sf::RenderTexture> m_paletteTexture;
	sf::View m_cam;

	std::vector<Palette> m_palettesList;
public:
	PaletteInterface()
		: m_paletteTexture()
		, m_cam(sf::FloatRect(0.f, 0.f, 256.f, 256.f))
	{
		// Initialisation de la caméra
		m_cam.setCenter(128.f, 128.f);
		m_cam.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

		m_paletteTexture = std::make_unique<sf::RenderTexture>();
		m_paletteTexture->create(SCREEN_WIDTH, SCREEN_HEIGHT);

		// Le vecteur m_palettesList est automatiquement initialisé vide
	}

	void Load(void);
	void Update(sf::RenderWindow& _window, float _dt);
};
