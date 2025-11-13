#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>
#include <filesystem>

class Map
{

public:
	using MapData = std::vector<std::vector<char>>;

	Map() = default;
	Map(const std::string& filePath);
	void Load();
	void Load(const std::string& filePath);
	bool HasChanged();
	MapData Data;

private:
	std::filesystem::file_time_type _lastChecked;
	std::string _filePath;
};

#endif
