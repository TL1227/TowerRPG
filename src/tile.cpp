#include "tile.h"

Tile::Tile(glm::vec3 position)
	: Position{ position }
{
	X = Position.x;
	Z = Position.z;
};
