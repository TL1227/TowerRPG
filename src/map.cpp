#include "map.h"

#include <fstream>
#include <filesystem>

Map::Map(const std::string& filePath) : _filePath{ filePath }
{
    _lastChecked = std::filesystem::last_write_time(_filePath);
    Load();
}

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

void Map::Load(const std::string& filePath)
{
    _filePath = filePath;
    Load();
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
