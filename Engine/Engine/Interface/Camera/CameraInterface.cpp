#include "CameraInterface.hpp"

#include "../../Camera/Camera.hpp"

#include <ImGui/imgui.h>
#include <SFML/System/Vector3.hpp>

void Engine::CameraInterface::Update(Camera* _cam)
{
	ImGui::Begin("Camera");

	sf::Vector3f camPos = _cam->GetPos();
	sf::Vector2f camSize = _cam->GetSize();
	float camAngle = _cam->GetAngle();
	bool camFree = _cam->GetFree();

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
	ImGui::Text("Rotation ");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(190);
	ImGui::DragFloat("##rotation", &camAngle, 0.1f);

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
