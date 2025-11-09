#ifndef GAME__HPP
#define GAME__HPP

#include <Engine/Scene/Scene.hpp>

#include "../../Common.h"
#include "Engine/TextureSlice/TextureSlice.hpp"

namespace Engine
{
	class Camera;
}

class Game : public Engine::Scene
{
private:
	Engine::Camera* m_camera;

	sf::Sprite m_tileSheet;
	float m_timer = 0.f;
	int m_searchID = 1;

	int map[500][500] = { 0 };
	std::vector<sf::Sprite> m_tileSprites;

	std::vector<Engine::TextureSliced> m_textureSliced;
public:
	Game();
	virtual ~Game();

	virtual void Init() override;
	virtual void PollEvents(sf::RenderWindow& _window, sf::Event& _event) override;
	virtual void Update(sf::RenderWindow& _renderWindow, float _dt) override;
	virtual void Display(sf::RenderWindow& _window) override;
	virtual void Cleanup() override;
};
#endif