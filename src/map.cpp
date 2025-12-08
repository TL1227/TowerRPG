#include "map.h"
#include "model.h"

#include <fstream>
#include <filesystem>

Map::Map(const std::string& filePath) : _filePath{ filePath }
{
    _lastChecked = std::filesystem::last_write_time(_filePath);
    Load();
}

void Map::Draw(glm::mat4 view)
{
    Shader.use();
    Shader.setMat4("view", view);

    //render level
    for (auto& tile : Tiles)
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), tile.Position);
        Shader.setMat4("model", model);

        if (tile.Model)
        {
            tile.Model->Draw(Shader);
        }
    }

    //floor TODO: create a floor tile and add them to the towertool map maker
    for (int i = 0; i < Data.size(); i++)
        for (int j = 0; j < Data[0].size(); j++)
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(j, -1.0f, i));
            Shader.setMat4("model", model);
            WallModel.Draw(Shader);
        }
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

    ChestModel = {"models\\chest\\wooden_crate_01_4k.gltf" };
    WallModel = {"models\\cube\\cube.gltf" };

    int rowSize = Data.size();
    int colSize = Data[0].size();

    for (int i = 0; i < rowSize; i++)
		for (int j = 0; j < colSize; j++)
        {
            if (Data[i][j] == ' ')
                continue;

			Tile tile{ glm::vec3{ j, 0.0f, i } };

            if (Data[i][j] == '#')
            {
                tile.TileType = TileType::Wall;
                tile.Model = &WallModel;
           }
            if (Data[i][j] == 'c')
            {
                tile.TileType = TileType::Chest;
                tile.Model = &ChestModel;
                tile.InteractiveText = "[E] Open"; //TODO: get this dynamically
            }
            else if (Data[i][j] == 's')
			{
                tile.TileType = TileType::Start;
                tile.IsWalkable = true;

                PlayerStartPos = glm::vec3{ j, 0.0f, i };
			}

			Tiles.push_back(tile);
        }

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 960.0f / 540.0f, 0.1f, 100.0f);
    Shader.use();
    Shader.setMat4("projection", projection);
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

char Map::GetChar(int x, int z)
{
	return Data[z][x];
}

Tile* Map::GetTile(int x, int z)
{
    for (auto& tile : Tiles)
        if (tile.X == x && tile.Z == z)
            return &tile;

    return nullptr;
}

Tile* Map::GetTile(glm::vec3 pos)
{
    for (auto& tile : Tiles)
        if (tile.X == pos.x && tile.Z == pos.z)
            return &tile;

    return nullptr;
}
