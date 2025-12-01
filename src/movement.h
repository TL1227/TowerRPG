#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <glm/glm.hpp>

#include "camera.h"
#include "map.h"
#include "enemy.h"
#include "battlemessagelistener.h"

enum class MoveAction
{
	Forwards,
	Backwards,
	Left,
	Right,
	TurnRight,
	TurnLeft,
	TurnAround,
	AutoTurnRight,
	AutoTurnLeft,
	AutoTurnAround,
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

class Movement : BattleMessageListener
{
public:
	Movement(Map& map, Camera& c);
    Cardinal GetNextRightDir() const;
    Cardinal GetNextLeftDir() const;
    Cardinal GetOppositeDir() const;
	void SetMoveAction(MoveAction);
	void MoveChar(float DeltaTime);
	bool IsStill() const;
	void EndBattle();
	glm::vec3 GetNextTile(MoveAction action);
	glm::vec3 GetNextEnemyTile(MoveAction action);

	Tile* FrontTile;
	float DistanceMoved = 0.0f;

	Enemy* Enemy;
	BattleMessage* BattleMessage;
    
    overide 

private:
	glm::vec3 DirOffset(Cardinal dir);
	Map& Map;
	Camera& Camera;
	Cardinal CurrentDirection = Cardinal::East;
	const float MovementUnit = 1.0f;
	const float NormalMovementSpeed = 2.0f;
	const float PreBattleMovementSpeed = 1.0f;
	float MovementSpeed = 2.0f;
	const float RotationSpeed = 150.0f;
	MoveAction CurrMovement = MoveAction::None;
	std::string PrintCurrentDirection();
	int EnemyCounter = 5;
	void EndMovement();
	void EndTurnMovement();
	bool IsAutoMove(MoveAction);
};

#endif
