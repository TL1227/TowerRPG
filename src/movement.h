#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <glm/glm.hpp>

#include "camera.h"
#include "map.h"
#include "enemy.h"

enum class MoveAction
{
	Forwards,
	Backwards,
	Left,
	Right,
	TurnRight,
	TurnLeft,
	TurnAround,
	None,
};

struct TilePos
{
	int X;
	int Z;
};

enum class Cardinal
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
    Cardinal GetNextRightDir() const;
    Cardinal GetNextLeftDir() const;
    Cardinal GetOppositeDir() const;
	void SetSurroundingTiles();
	void SetMoveAction(MoveAction);
	void MoveChar(float DeltaTime);
	bool IsStill() const;
	float GetDirection();
	void EndBattle();

	vector<glm::vec3> SurroundingTiles{4};
	vector<glm::vec3> EnemyTiles{4};

	Tile* FrontTile;
	float DistanceMoved = 0.0f;
	bool WeBattleNow = false;
	Enemy* Enemy;

private:
	glm::vec3 DirOffset(Cardinal dir);
	Map& Map;
	Camera& Camera;
	Cardinal CurrentDirection = Cardinal::East;
	float MovementUnit = 1.0f;
	const float NormalMovementSpeed = 2.0f;
	const float PreBattleMovementSpeed = 1.0f;
	float MovementSpeed = 2.0f;
	const float RotationSpeed = 150.0f;
	MoveAction CurrMovement = MoveAction::None;
	std::string PrintDir();
	int EnemyCounter = 5;
	void EndMovement();
	glm::vec3 GetNextMoveActionTilePos(MoveAction action);
};

#endif
