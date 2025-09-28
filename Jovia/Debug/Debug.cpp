#include "Debug.h"

void Debug::Update(float _dt, WindowState& _windowState, sf::RenderWindow& _window)
{
	ImGui::Begin("Debug");
	ImGui::Text("FPS : %.2f", 1 / _dt);


	if (ImGui::Checkbox("Vsync", &_windowState.vsync))
	{
		_window.setVerticalSyncEnabled(_windowState.vsync);
	}

	if (ImGui::Checkbox("Fullscreen", &_windowState.fullscreen))
	{
		_windowState.requestFullscreen = true;
	}

	ImGui::Separator();

	sf::Vector2f mouseDelta = { ImGui::GetIO().MouseDelta.x, ImGui::GetIO().MouseDelta.y };

	if (ImGui::TreeNode("Inputs"))
	{
		sf::Vector2i mousePosPixel = sf::Mouse::getPosition(_window);
		sf::Vector2f mousePose = _window.mapPixelToCoords(mousePosPixel);

		ImGui::Text("Mouse Pos : (%.f,", mousePose.x);
		ImGui::SameLine();
		ImGui::Text("%.f)", mousePose.y);
		ImGui::Text("Mouse delta: (%g, %g)", mouseDelta.x, mouseDelta.y);

		ImGui::TreePop();
	}

	ImGui::End();
}
