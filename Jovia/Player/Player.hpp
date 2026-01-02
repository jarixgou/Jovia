#ifndef PLAYER__HPP
#define PLAYER__HPP

#include <Engine/GameObject/GameObject.hpp>

class Player : public Engine::GameObject
{
private:
	
public:
	Player();

	void Update() override;
	void Display() override;
};

#endif