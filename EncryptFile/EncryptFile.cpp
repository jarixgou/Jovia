#include <iostream>
#include <fstream>

int main()
{
	// Key to use : uqMq*jj_!mhx#t],9x!(V0Su/#>]*`4>^!+~7HJ^F;d)I`bYi-Bzsfcb'=F"3)_

	std::fstream inputFile;
	std::fstream outputFile;

	std::string filePath = { 0 };
	std::string fileName = { 0 };
	std::string keyString = { 0 };
	int keyValue = 0;

	std::cout << "Enter the key : ";
	std::cin >> keyString;

	for (char string : keyString)
	{
		keyValue += string;
	}

	std::cout << "Key value : " << keyValue << std::endl;

	while (1)
	{
		std::cout << "Enter the file path : ";
		std::cin >> filePath;

		// Get the file name
		size_t dotPos = filePath.find_last_of(".");
		if (dotPos != std::string::npos)
		{
			fileName = filePath.substr(0, dotPos);
		}

		inputFile.open(filePath, std::ios::in | std::ios::binary);
		outputFile.open(fileName + ".asset", std::ios::out | std::ios::binary);

		if (!inputFile.is_open() || !outputFile.is_open())
		{
			std::cout << "Error to opening file";
			system("pause");
			return EXIT_FAILURE;
		}

		char charOutput = 0;
		while (inputFile.read(&charOutput, 1))
		{
			char encrypted = (unsigned char)(charOutput + keyValue);
			outputFile.write(&encrypted, 1);
		}

		inputFile.close();
		outputFile.close();
		inputFile.clear();
		outputFile.clear();

		std::cout << "finished to encrypt file !" << std::endl;
	}

	return EXIT_SUCCESS;
}