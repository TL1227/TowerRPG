#include "tile.h"

Tile::Tile(glm::vec3 position)
	: Position{ position }
{
	X = (int)Position.x;
	Z = (int)Position.z;
};
