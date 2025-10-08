#include "PaletteInterface.h"

#include <commdlg.h>
#include <tchar.h>

void PaletteInterface::Load()
{
	this->m_paletteTexture = std::make_unique<sf::RenderTexture>();
	this->m_paletteTexture->create(SCREEN_WIDTH, SCREEN_HEIGHT);
	this->m_cam.setCenter(0, 0);
}

void PaletteInterface::Update(sf::RenderWindow& _window, float _dt)
{
	ImGui::Begin("Palette");

	ImVec2 avail = ImGui::GetContentRegionAvail();
	sf::Vector2u size = this->m_paletteTexture->getSize();
	sf::Vector2f textureSize = { static_cast<float>(size.x), static_cast<float>(size.y) };
	sf::Vector2f interfaceSize = { avail.x, avail.y};

	if (interfaceSize.x > 1 && interfaceSize.y > 1 && textureSize != interfaceSize)
	{
		this->m_paletteTexture->create(static_cast<unsigned int>(interfaceSize.x), 
			static_cast<unsigned int>(interfaceSize.y));
		this->m_cam.setSize(interfaceSize);
		textureSize = interfaceSize;
	}

	this->m_paletteTexture->setView(this->m_cam);

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
				palette.m_rectsList[y][x] = Utils::ModifiableRect({ static_cast<float>(y * 32), static_cast<float>(x * 32) }, 
					{ 32,32 });
			}
		}

		this->m_palettesList.push_back(palette);
	}

	ImGuiTabBarFlags tabBarFlags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar("TabBar", tabBarFlags))
	{
		for (auto& palette : this->m_palettesList)
		{
			if (ImGui::BeginTabItem(palette.m_name.c_str()))
			{
				this->m_paletteTexture->clear(sf::Color::Black);

				this->m_paletteTexture->draw(palette.m_sprite);

				for (auto & mRectsList : palette.m_rectsList)
				{
					for (auto & modifiableRect : mRectsList)
					{
						this->m_paletteTexture->draw(modifiableRect.GetShape());
					}
				}

				this->m_paletteTexture->display();

				ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(this->m_paletteTexture->getTexture().getNativeHandle())), 
				             avail, ImVec2(0, 1), ImVec2(1, 0));
				ImGui::EndTabItem();
			}
		}
	}
	ImGui::EndTabBar();

	ImGui::End();
}
