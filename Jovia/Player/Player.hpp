#ifndef PLAYER__HPP
#define PLAYER__HPP

#include <Engine/GameObject/GameObject.hpp>

class Player : public Engine::GameObject
{
private:
	
public:
	Player();
	~Player() override;

	void Update() override;
	void Display() override;
};

#endif