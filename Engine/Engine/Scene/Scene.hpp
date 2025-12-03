#ifndef SCENE__HPP
#define SCENE__HPP
#include "SFML/Graphics/RenderWindow.hpp"

namespace Engine
{
	class Scene
	{
	public:
		Scene();
		virtual ~Scene();

		virtual void Init();
		virtual void PollEvents(sf::Event& _event);
		virtual void Update(float _dt);
		virtual void Display();
		virtual void Cleanup();
	};
}

#endif