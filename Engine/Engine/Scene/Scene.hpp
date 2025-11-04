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
		virtual void PollEvents(sf::RenderWindow& _window, sf::Event& _event);
		virtual void Update(sf::RenderWindow& _renderWindow, float _dt);
		virtual void Display(sf::RenderWindow& _window);
		virtual void Cleanup();
	};
}

#endif