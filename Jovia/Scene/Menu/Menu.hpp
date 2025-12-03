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
	virtual void PollEvents(sf::Event& _event) override;
	virtual void Update(float _dt) override;
	virtual void Display() override;
	virtual void Cleanup() override;
};

#endif