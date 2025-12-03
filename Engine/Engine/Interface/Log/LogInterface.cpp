#include "LogInterface.hpp"

#include <ImGui/imgui.h>

#include "../../Logger/Logger.hpp"

namespace Engine
{
	namespace LogInterface
	{
		void Update()
		{
			ImGui::Begin("Log");

			ImGui::BeginChild("##Child");

			for (auto& log : Logger::GetLogHistory())
			{
				ImGui::Text(log.c_str());
			}

			ImGui::EndChild();

			ImGui::End();
		}
	}
}
