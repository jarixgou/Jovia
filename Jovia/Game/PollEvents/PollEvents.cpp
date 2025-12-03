#include "PollEvents.hpp"

#include <Engine/Scene/ScenesManager.hpp>

#include <Engine/System/System.hpp>

void PollEvents()
{
	sf::Event event;
	while (System::window->pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(*System::window, event);
		if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
		{
			System::window->close();
		}

		Engine::ScenesManager::PollEvents(event);
	}
}
