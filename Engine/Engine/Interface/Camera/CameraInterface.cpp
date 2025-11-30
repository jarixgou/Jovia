#include "CameraInterface.hpp"

#include "../../Camera/Camera.hpp"

#include <ImGui/imgui.h>
#include <SFML/System/Vector3.hpp>

void Engine::CameraInterface::Update(Camera* _cam)
{
	ImGui::Begin("Camera");

	sf::Vector3f camPos = _cam->GetPos();
	sf::Vector2f camSize = _cam->GetSize();
	sf::Vector3f camAngle = _cam->GetAngle();
	CameraType camType = _cam->GetType();
	bool camFree = _cam->GetFree();

	std::string allTypeName[2] = {"Orthogonal", "Isometric"};

	int currentIdx = 0;
	if (camType == CameraType::ORTHOGRAPHIC)
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
					_cam->SetType(CameraType::ORTHOGRAPHIC);
				}
				else if (allTypeName[i] == "Isometric")
				{
					_cam->SetType(CameraType::ISOMETRIC);
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
	ImGui::DragFloat("##angleX", &camAngle.x, 0.1f);
	ImGui::PopStyleColor();

	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.2f, 0.4f, 0.2f, 1.0f));
	ImGui::SetNextItemWidth(60);
	ImGui::DragFloat("##angleY", &camAngle.y, 0.1f);
	ImGui::PopStyleColor();

	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.2f, 0.2f, 0.4f, 1.0f));
	ImGui::SetNextItemWidth(60);
	ImGui::DragFloat("##angleZ", &camAngle.z, 0.1f);
	ImGui::PopStyleColor();

	// Free
	ImGui::Text("Free    ");
	ImGui::SameLine();
	ImGui::Checkbox("##Free Camera", &camFree);

	_cam->SetPos(camPos);
	_cam->SetSize(camSize);
	_cam->SetFree(camFree);
	_cam->SetAngle(camAngle);

	ImGui::End();
}
