#include "Update.hpp"

#include <Engine/Scene/ScenesManager.hpp>
#include <Engine/System/System.hpp>


void Update()
{
	ImGui::SFML::Update(*Engine::System::window, Engine::System::time.GetTime());

	Engine::ScenesManager::Update();
}
