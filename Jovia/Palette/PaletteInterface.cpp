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
		//OPENFILENAME ofn = { 0 };
		//TCHAR szFile[FILENAME_MAX] = { 0 };

		//ofn.lStructSize = sizeof(ofn);
		//ofn.lpstrFile = szFile;
		//ofn.nMaxFile = sizeof(szFile);
		//ofn.lpstrFilter = TEXT("All\0*.*\0Text\0*.TXT\0");
		//ofn.nFilterIndex = 1;
		//ofn.lpstrFileTitle = NULL;
		//ofn.nMaxFileTitle = 0;
		//ofn.lpstrInitialDir = NULL;
		//ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		//if (GetOpenFileName(&ofn) == TRUE)
		//{

		//}

		Palette palette;
		palette.m_name = "Test";
		palette.m_texture.loadFromFile("Assets/spritesheet.png");
		palette.m_sprite.setTexture(palette.m_texture);

		int row = palette.m_texture.getSize().x / 32;
		int column = palette.m_texture.getSize().y / 32;

		palette.m_rectsList.resize(row);
		for (size_t y = 0; y < palette.m_rectsList.size(); ++y)
		{
			palette.m_rectsList[y].resize(column);
			for (size_t x = 0; x < palette.m_rectsList[y].size(); ++x)
			{
				Utils::ModifiableRect& temp = palette.m_rectsList[y][x];
				temp = Utils::ModifiableRect({ static_cast<float>(y * 32), static_cast<float>(x * 32) }, { 32,32 });

			}
		}
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
