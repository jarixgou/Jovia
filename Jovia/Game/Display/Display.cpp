#include "Display.hpp"

#include <Engine/Scene/ScenesManager.hpp>

void Display(sf::RenderWindow& _window)
{
	_window.clear(sf::Color::Green);

	Engine::ScenesManager::Display(_window);
	ImGui::SFML::Render(_window);
	_window.display();
}
