#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>
#include <filesystem>

class Map
{

public:
	using MapData = std::vector<std::vector<char>>;

	Map(const std::string& filePath)
		: _filePath{ filePath }
	{
		_lastChecked = std::filesystem::last_write_time(_filePath);
		Load();
	}

	void Load();
	bool HasChanged();
	MapData Data;

private:
	std::filesystem::file_time_type _lastChecked;
	std::string _filePath;
};

#endif
