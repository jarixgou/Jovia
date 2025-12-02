#ifndef PLAYER__HPP
#define PLAYER__HPP

#include <Engine/GameObject/GameObject.hpp>

class Player : public Engine::GameObject
{
private:

public:
	void Init() override;
	void Update(const float& _dt) override;
	void Display(const Engine::Camera* _camera) override;
};

#endif