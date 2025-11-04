#include "Update.hpp"

#include <Engine/Scene/ScenesManager.hpp>

#include "Engine/Interface/Assets/AssetsInterface.hpp"

void Update(sf::RenderWindow& _window, const sf::Time& _time, float _dt)
{
	ImGui::SFML::Update(_window, _time);

	Engine::AssetsInterface::Update();

	Engine::ScenesManager::Update(_window, _dt);
}
