#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <glm/glm.hpp>

#include "camera.h"
#include "map.h"
#include "enemy.h"
#include "battleeventlistener.h"
#include "battlesystem.h"
#include "movementevent.h"
#include "cardinal.h"
#include "inputeventlistener.h"

//TODO: move this to it's own file one day

class MovementSystem : public BattleEventListener, public InputEventListener
{
public:
	MovementSystem(Map& map, Camera& c);
    Cardinal GetNextRightDir() const;
    Cardinal GetNextLeftDir() const;
    Cardinal GetOppositeDir() const;
	void ProcessMoveAction(MoveAction);
	void Tick(float DeltaTime);
	bool IsStill() const;
	glm::vec3 GetNextTile(MoveAction action);
	glm::vec3 GetNextEnemyTile(MoveAction action);

	Tile* FrontTile;
	float DistanceMoved = 0.0f;

	Enemy* Enemy;
	BattleSystem* BattleSystem;

	MovementEvent* Event;
    
	void OnBattlePhaseChange(BattlePhase bp) override;
	void OnMoveActionButtonPress(MoveAction action) override;

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
	MoveAction CurrentMoveAction = MoveAction::None;
	MoveAction InputMoveAction = MoveAction::None;
	std::string PrintCurrentDirection();
	void EndMovement();
	void EndTurnMovement();
	bool IsAutoMove(MoveAction);
	BattlePhase CurrentBattlePhase;
	void SetCurrentMoveAction(MoveAction);
	void SetCurrentDirection(Cardinal);
	void SetMoveDistance(float);
	bool CanMove();
};

#endif
