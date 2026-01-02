#include "SystemInterface.hpp"

#include <ImGui/imgui.h>

#include "../../Render/RenderAPI.hpp"
#include "../../System/System.hpp"


namespace Engine
{
	namespace SystemInterface
	{
		void Update()
		{
			ImGui::Begin("System");

			ImGui::Text("RenderAPI");
			ImGui::SameLine();
			if (ImGui::Checkbox("##RenderAPI", &System::drawLight))
			{
				if (System::drawLight)
				{
					RenderAPI::Init();
				}
				else
				{
					RenderAPI::Cleanup();
				}
			}

			float clockSpeed = System::time.GetClockSpeed();
			ImGui::Text("Clock : ");
			ImGui::SameLine();
			ImGui::DragFloat("##Clock", &clockSpeed, 0.01f, 0.0f, 100.f);
			ImGui::Text("Vertices numbers : %d", System::verticeNb);
			ImGui::Text("Draw call :       %d", System::drawCall);

			ImGui::End();

			System::time.SetClockSpeed(clockSpeed);
		}
	}
}
