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

            // Pré-calcul des compteurs (optionnel, pour affichage)
            int loadedCount = 0, unloadedCount = 0;
            for (const auto& asset : assetList)
            {
                if (asset->m_load) ++loadedCount;
                else ++unloadedCount;
            }

            if (ImGui::TreeNode("Loaded", "Loaded (%d)", loadedCount))
            {
                for (const auto& asset : assetList)
                {
                    if (asset->m_load)
                        ImGui::BulletText("%s", asset->m_path.c_str());
                }
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Not loaded", "Not loaded (%d)", unloadedCount))
            {
                for (const auto& asset : assetList)
                {
                    if (!asset->m_load)
                        ImGui::BulletText("%s", asset->m_path.c_str());
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