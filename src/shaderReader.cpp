#include "shaderReader.h"

#include <fstream>
#include <string>

std::string ReadShaderFile(const char* filePath)
{
	std::string out;

	std::ifstream file(filePath);
	std::string buff{};

	while (getline(file, buff))
	{
		buff.push_back('\n');
		out += buff;
	}

	return out;
}
