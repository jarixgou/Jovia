#include "Update.hpp"

#include <thread>
#include <Engine/Scene/ScenesManager.hpp>

#include <Engine/Interface/Assets/AssetsInterface.hpp>
#include <Engine/Interface/Debug/DebugInterface.hpp>
#include <Engine/Interface/System/SystemInterface.hpp>
#include <Engine/System/System.hpp>

#include "Engine/Interface/Log/LogInterface.hpp"

void Update(sf::Time _time, float _dt)
{
	ImGui::SFML::Update(*System::window, _time);

	Engine::ScenesManager::Update(_dt);

	Engine::AssetsInterface::Update();
	Engine::SystemInterface::Update();
	Engine::LogInterface::Update();
}
