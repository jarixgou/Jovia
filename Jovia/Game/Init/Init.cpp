#include "Init.hpp"

#include <Engine/Scene/ScenesManager.hpp>

#include "../../Scene/Menu/Menu.hpp"
#include "../../Scene/Game/Game.hpp"

void InitSystem(sf::RenderWindow& _window);
void InitScene();

void Init(sf::RenderWindow& _window)
{
	InitSystem(_window);
	InitScene();
}

void InitSystem(sf::RenderWindow& _window)
{
	_window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Jovia");
	LOG_DEBUG("Render window was created successfully");

	ImGui::SFML::Init(_window);
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	LOG_DEBUG("ImGui was initialized");
}

void InitScene()
{
	Engine::ScenesManager::CreateScene<Menu>("Menu");
	Engine::ScenesManager::CreateScene<Game>("Game");

	Engine::ScenesManager::LoadScene("Menu");
}