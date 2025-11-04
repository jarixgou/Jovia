#ifndef MENU__HPP
#define MENU__HPP

#include <Engine/Scene/Scene.hpp>

#include "../../Common.h"

class Menu : public Engine::Scene
{
private:

public:
	Menu();
	virtual ~Menu();

	virtual void Init() override;
	virtual void PollEvents(sf::RenderWindow& _window, sf::Event& _event) override;
	virtual void Update(sf::RenderWindow& _renderWindow, float _dt) override;
	virtual void Display(sf::RenderWindow& _window) override;
	virtual void Cleanup() override;
};

#endif