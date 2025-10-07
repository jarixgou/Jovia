#include "PaletteInterface.h"

#include <commdlg.h>
#include <tchar.h>

void PaletteInterface::Load()
{
	this->m_paletteTexture = std::make_unique<sf::RenderTexture>();
	this->m_paletteTexture->create(256, 256);
}

void PaletteInterface::Update(sf::RenderWindow& _window, float _dt)
{
	ImGui::Begin("Palette");

	if (ImGui::Button("new palette"))
	{
		/*OPENFILENAME ofn = { 0 };
		TCHAR szFile[FILENAME_MAX] = { 0 };

		ofn.lStructSize = sizeof(ofn);
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = TEXT("All\0*.*\0Text\0*.TXT\0");
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetOpenFileName(&ofn) == TRUE)
		{

		}*/


	}

	ImGuiTabBarFlags tabBarFlags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar("TabBar", tabBarFlags))
	{
		for (auto& palette : this->m_palettesList)
		{
			if (ImGui::BeginTabItem(palette.m_name.c_str()))
			{
				palette.m_selected = true;
			}
			else
			{
				palette.m_selected = false;
			}
		}
	}
	ImGui::EndTabBar();

	ImGui::End();
}
