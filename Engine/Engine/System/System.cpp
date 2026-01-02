#include "System.hpp"

#include <fstream>
#include <ios>

namespace Engine
{
	std::unique_ptr<sf::RenderWindow> System::window = nullptr;
	bool System::drawLight = false;
	int System::drawCall = 0;
	int System::verticeNb = 0;
	Camera** System::currentCamera = nullptr;
	Time System::time;

	std::string ChangeFileFormat(std::string _filePath)
	{
		std::string result;
		std::fstream file(_filePath, std::ios::in | std::ios::binary);

		if (!file.is_open())
		{
			return result;
		}

		char charOutput = 0;
		while (file.read(&charOutput, 1))
		{
			int keyValue = 4856;
			unsigned char temp = static_cast<unsigned char>(charOutput - keyValue);
			result += temp;
		}

		file.close();
		file.clear();

		return result;
	}
}