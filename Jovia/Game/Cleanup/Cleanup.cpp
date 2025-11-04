#include "Cleanup.hpp"

#include <Engine/Scene/ScenesManager.hpp>
#include <Engine/Asset/AssetsManager.hpp>

void Cleanup()
{
	ImGui::SFML::Shutdown();
	Engine::ScenesManager::Cleanup();
	Engine::AssetsManager::Clear();
}
