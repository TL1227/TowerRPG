#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>
#include <filesystem>

#include "tile.h"
#include "shader.h"

class Map
{

public:
	using MapData = std::vector<std::vector<char>>;

	Map() = default;
	Map(const std::string& filePath);
	void Load();
	void Load(const std::string& filePath);
    void Draw(glm::mat4 view);
	bool HasChanged();
	char GetChar(int x, int z);
	Tile* GetTile(int x, int z);
	Tile* GetTile(glm::vec3 pos);
	MapData Data;
	vector<Tile> Tiles;
	glm::vec3 PlayerStartPos;

	//TODO: Store these a bit more elegantly 
	Model ChestModel;
	Model WallModel;

private:
	std::filesystem::file_time_type _lastChecked;
	std::string _filePath;
    Shader Shader{ "shaders\\vert.shader", "shaders\\frag.shader" };
};

#endif
