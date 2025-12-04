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

			ImGui::Text("Vertices numbers : %d", System::verticeNb);
			ImGui::Text("Draw call :       %d", System::drawCall);

			ImGui::End();
		}
	}
}
