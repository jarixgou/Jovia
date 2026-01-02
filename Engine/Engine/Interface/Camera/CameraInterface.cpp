#include "CameraInterface.hpp"

#include "../../Camera/Camera.hpp"

#include <ImGui/imgui.h>
#include <SFML/System/Vector3.hpp>

#include "../../System/System.hpp"

void Engine::CameraInterface::Update()
{
	if (System::currentCamera != nullptr)
	{
		ImGui::Begin("Camera");

		Transform camTransform = (*System::currentCamera)->GetTransform();

		sf::Vector3f& camPos = camTransform.position;
		sf::Vector3f& camSize = camTransform.size;
		sf::Vector3f& rotation = camTransform.rotation;
		CameraType camType = (*System::currentCamera)->GetType();
		bool camFree = (*System::currentCamera)->GetFree();

		std::string allTypeName[2] = { "Orthogonal", "Isometric" };

		int currentIdx = 0;
		if (camType == CameraType::ORTHOGONAL)
		{
			currentIdx = 0;
		}
		else if (camType == CameraType::ISOMETRIC)
		{
			currentIdx = 1;
		}

		// Type
		ImGui::Text("Type    ");
		ImGui::SameLine();
		if (ImGui::BeginCombo("##Type", allTypeName[currentIdx].c_str()))
		{
			for (int i = 0; i < 2; ++i)
			{
				const bool isSelected = (currentIdx == i);
				if (ImGui::Selectable(allTypeName[i].c_str(), isSelected))
				{
					currentIdx = i;
					if (allTypeName[i] == "Orthogonal")
					{
						(*System::currentCamera)->SetType(CameraType::ORTHOGONAL);
					}
					else if (allTypeName[i] == "Isometric")
					{
						(*System::currentCamera)->SetType(CameraType::ISOMETRIC);
					}
				}

				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}

			ImGui::EndCombo();
		}

		// Position
		ImGui::Text("Position");
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.4f, 0.2f, 0.2f, 1.0f));
		ImGui::SetNextItemWidth(60);
		ImGui::DragFloat("##posX", &camPos.x, 0.1f);
		ImGui::PopStyleColor();

		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.2f, 0.4f, 0.2f, 1.0f));
		ImGui::SetNextItemWidth(60);
		ImGui::DragFloat("##posY", &camPos.y, 0.1f);
		ImGui::PopStyleColor();

		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.2f, 0.2f, 0.4f, 1.0f));
		ImGui::SetNextItemWidth(60);
		ImGui::DragFloat("##posZ", &camPos.z, 0.1f);
		ImGui::PopStyleColor();

		// Size
		ImGui::Text("Size    ");
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.4f, 0.2f, 0.2f, 1.0f));
		ImGui::SetNextItemWidth(60);
		ImGui::DragFloat("##sizeX", &camSize.x, 1.0f);
		ImGui::PopStyleColor();

		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.2f, 0.4f, 0.2f, 1.0f));
		ImGui::SetNextItemWidth(60);
		ImGui::DragFloat("##sizeY", &camSize.y, 1.0f);
		ImGui::PopStyleColor();

		// Rotation
		ImGui::Text("Rotation");
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.4f, 0.2f, 0.2f, 1.0f));
		ImGui::SetNextItemWidth(60);
		ImGui::DragFloat("##angleX", &rotation.x, 0.1f);
		ImGui::PopStyleColor();

		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.2f, 0.4f, 0.2f, 1.0f));
		ImGui::SetNextItemWidth(60);
		ImGui::DragFloat("##angleY", &rotation.y, 0.1f);
		ImGui::PopStyleColor();

		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.2f, 0.2f, 0.4f, 1.0f));
		ImGui::SetNextItemWidth(60);
		ImGui::DragFloat("##angleZ", &rotation.z, 0.1f);
		ImGui::PopStyleColor();

		// Free
		ImGui::Text("Free    ");
		ImGui::SameLine();
		ImGui::Checkbox("##Free Camera", &camFree);

		(*System::currentCamera)->SetTransform(camTransform);
		(*System::currentCamera)->SetFree(camFree);

		ImGui::End();
	}
}
