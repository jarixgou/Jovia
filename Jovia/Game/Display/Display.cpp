#include "Display.hpp"

#include <Engine/Scene/ScenesManager.hpp>

#include "Engine/System/System.hpp"

void Display()
{
	System::window->clear(sf::Color::Black);

	Engine::ScenesManager::Display();
	ImGui::SFML::Render(*System::window);

	System::window->display();
}
