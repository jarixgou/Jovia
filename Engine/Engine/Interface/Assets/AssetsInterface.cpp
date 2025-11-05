#include "AssetsInterface.hpp"

#include <ImGui/imgui.h>
#include "../../Asset/AssetsManager.hpp"

namespace Engine
{
	namespace AssetsInterface
	{
		void Update()
		{
			ImGui::Begin("Asset");

			ImGui::BeginChild("Assets");
			const auto& assetList = AssetsManager::GetList();

			if (ImGui::TreeNode("Loaded"))
			{
				for (const auto& asset : assetList)
				{
					if (asset->m_load)
					{
						ImGui::BulletText(asset->m_path.c_str());
					}
				}
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Not loaded"))
			{
				for (const auto& asset : assetList)
				{
					if (!asset->m_load)
					{
						ImGui::BulletText(asset->m_path.c_str());
					}
				}
				ImGui::TreePop();
			}
			ImGui::EndChild();

			ImGui::SameLine();

			ImGui::BeginChild("Preview");

			ImGui::EndChild();

			ImGui::End();
		}
	}
}