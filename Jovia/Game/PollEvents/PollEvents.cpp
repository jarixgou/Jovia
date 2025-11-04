#include "PollEvents.hpp"

#include <Engine/Scene/ScenesManager.hpp>


void PollEvents(sf::RenderWindow& _window)
{
	sf::Event event;
	while (_window.pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(_window, event);
		if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
		{
			_window.close();
		}

		Engine::ScenesManager::PollEvents(_window, event);
	}
}
