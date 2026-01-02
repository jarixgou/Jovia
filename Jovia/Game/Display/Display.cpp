#include "Display.hpp"

#include <Engine/Scene/ScenesManager.hpp>

#include "Engine/System/System.hpp"

void Display()
{
	Engine::System::window->clear(sf::Color::Black);

	Engine::System::verticeNb = 0;
	Engine::System::drawCall = 0;

	Engine::ScenesManager::Display();
	ImGui::SFML::Render(*Engine::System::window);

	Engine::System::window->display();
}
