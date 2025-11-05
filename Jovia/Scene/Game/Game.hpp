#ifndef GAME__HPP
#define GAME__HPP

#include <Engine/Scene/Scene.hpp>

#include "../../Common.h"
#include "Engine/TextureSlice/TextureSlice.hpp"


class Game : public Engine::Scene
{
private:
	sf::Sprite tileSheet;
	float timer = 0.f;
	int searchID = 1;

	int map[50][50] = { 0 };

	std::vector<Engine::TextureSliced> textureSliced;
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