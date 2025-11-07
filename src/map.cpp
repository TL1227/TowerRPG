#include "map.h"

#include <fstream>
#include <filesystem>

void Map::Load()
{
	Data.clear();

	//TODO: add error handling if you can't read file
	std::ifstream file(_filePath);

	std::string line;
	while (std::getline(file, line))
	{
		Data.emplace_back(line.begin(), line.end());
	}
}

bool Map::HasChanged()
{
	auto currentCheck = std::filesystem::last_write_time(_filePath);

	if (currentCheck > _lastChecked)
	{
		_lastChecked = currentCheck;
		return true;
	}
	else
	{
		return false;
	}
}
