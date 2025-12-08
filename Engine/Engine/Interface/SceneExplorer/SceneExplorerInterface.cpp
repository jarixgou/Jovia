#include "SceneExplorerInterface.hpp"

#include <ImGui/imgui.h>

#include "../../Scene/ScenesManager.hpp"

namespace Engine
{
	namespace SceneExplorerInterface
	{
		void Update()
		{
			ImGui::Begin("Scene explorer");

			auto sceneList = ScenesManager::GetSceneList();

			for (auto & [name, scene] : sceneList)
			{
				if (ImGui::Selectable(name.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick))
				{
					if (ImGui::IsMouseDoubleClicked(0))
					{
						ScenesManager::LoadScene(name.c_str());
					}
				}
			}

			ImGui::End();
		}
	}
}
