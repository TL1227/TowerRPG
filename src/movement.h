#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <glm/glm.hpp>

#include "camera.h"
#include "map.h"

enum class MoveAction
{
	None,
	Forwards,
	Backwards,
	Left,
	Right,
	TurnRight,
	TurnLeft
};

struct TilePos
{
	int X;
	int Z;
};

struct SurroundingTiles
{
	glm::vec3 Front;
	glm::vec3 Back;
	glm::vec3 Left;
	glm::vec3 Right;
};

enum class Direction
{
	South = 90,
	West = 180,
	North = 270,
	East = 0,
};

class Movement
{
public:
	Movement(Map& map, Camera& c);
    Direction GetNextRightDir() const;
    Direction GetNextLeftDir() const;
	void SetSurroundingTiles();
	void SetMoveAction(MoveAction);
	void MoveChar(float DeltaTime);
	bool IsStill() const;
	glm::vec3 FrontTilePos;
	glm::vec3 BackTilePos;
	glm::vec3 LeftTilePos;
	glm::vec3 RightTilePos;
	Tile* FrontTile;

private:

	glm::vec3 DirOffset(Direction dir);
	Map& Map;
	Camera& Camera;
	Direction CurrentDirection = Direction::East;
	float DistanceMoved = 0.0f;
	float MovementUnit = 1.0f;
	const float MovementSpeed = 2.0f;
	const float RotationSpeed = 150.0f;
	MoveAction CurrMovement = MoveAction::None;
	std::string PrintDir();
};

#endif
