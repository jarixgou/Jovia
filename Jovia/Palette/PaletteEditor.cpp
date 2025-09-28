#include "PaletteEditor.h"

#include <commdlg.h>
#include <tchar.h>

void PaletteEditor::Update(sf::RenderWindow& _window, float _dt)
{
	ImGui::Begin("PaletteEditor");

	if (ImGui::Button("new palette"))
	{
		OPENFILENAME ofn = { 0 };
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

		}
	}

	ImGui::End();
}