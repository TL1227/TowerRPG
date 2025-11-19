#ifndef TILE_H
#define TILE_H

#include <iostream>

#include <glm/glm.hpp>
#include "model.h"

enum class TileType
{
    Wall,
    Chest,
    Door,
    Start
};

class Tile
{
    public:
        Tile(glm::vec3 position);
        
        Model* Model{};
        glm::vec3 Position;
        int X;
        int Z;
        TileType TileType{};
        bool IsWalkable{};
        std::string InteractiveText{};
};

#endif

