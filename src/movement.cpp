#include <iostream>
#include <fstream>
#include <random>

#include "movement.h"
#include "args.h"

static MoveAction NextMove = MoveAction::None;

Movement::Movement(::Map& map, ::Camera& camera)
	: Map{ map }, Camera{ camera }
{
}

//TODO:: This does NOT belong in this file. Move it later
static int RandomNumber()
{
	std::random_device rd;  // a seed source for the random number engine
	std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> distrib(3, 10);

	return distrib(gen);
}

Cardinal Movement::GetNextRightDir() const
{
	if (CurrentDirection == Cardinal::North)
		return Cardinal::East;
	if (CurrentDirection == Cardinal::East)
		return Cardinal::South;
	if (CurrentDirection == Cardinal::South)
		return Cardinal::West;
	if (CurrentDirection == Cardinal::West)
		return Cardinal::North;
}

Cardinal Movement::GetNextLeftDir() const
{
	if (CurrentDirection == Cardinal::North)
		return Cardinal::West;
	if (CurrentDirection == Cardinal::West)
		return Cardinal::South;
	if (CurrentDirection == Cardinal::South)
		return Cardinal::East;
	if (CurrentDirection == Cardinal::East)
		return Cardinal::North;
}

Cardinal Movement::GetOppositeDir() const
{
	if (CurrentDirection == Cardinal::North)
		return Cardinal::South;
	if (CurrentDirection == Cardinal::West)
		return Cardinal::East;
	if (CurrentDirection == Cardinal::South)
		return Cardinal::North;
	if (CurrentDirection == Cardinal::East)
		return Cardinal::West;
}

bool BlockIsSolid(char ch)
{
	return (ch == '#' || ch == 'c');
}

void Movement::SetMoveAction(MoveAction action)
{
	if (CurrMovement == MoveAction::None)
	{
		if (action == MoveAction::TurnRight ||
			action == MoveAction::TurnLeft ||
			action == MoveAction::TurnAround ||
			action == MoveAction::AutoTurnRight ||
			action == MoveAction::AutoTurnLeft ||
			action == MoveAction::AutoTurnAround)
		{
			CurrMovement = action;
			return;
		}

		Tile* nextTile = nullptr;

		nextTile = Map.GetTile(GetNextTile(action));
		if (!nextTile || nextTile->IsWalkable)
		{
			Tile* enemyTile = Map.GetTile(GetNextEnemyTile(action));
			if (!enemyTile || enemyTile->IsWalkable)
			{
				if (BattleMessage->CurrentPhase == BattlePhase::Sighting)
				{
					MovementSpeed = PreBattleMovementSpeed;
					Enemy->Position = GetNextEnemyTile(action);
					Enemy->PlayerDirection = (float)CurrentDirection;

					if (action == MoveAction::Left)
					{
						NextMove = MoveAction::AutoTurnLeft;
						Enemy->PlayerDirection = (float)GetNextLeftDir();
					}
					if (action == MoveAction::Right)
					{
						NextMove = MoveAction::AutoTurnRight;
						Enemy->PlayerDirection = (float)GetNextRightDir();
					}
					if (action == MoveAction::Backwards)
					{
						NextMove = MoveAction::AutoTurnAround;
						Enemy->PlayerDirection = (float)GetOppositeDir();
					}
				}
			}

			CurrMovement = action;
		}
	}
}

void Movement::EndBattle()
{
	MovementSpeed = NormalMovementSpeed;
}

std::string Movement::PrintCurrentDirection()
{
	switch (CurrentDirection)
	{
		case Cardinal::North: return "North";
		case Cardinal::South: return "South";
		case Cardinal::East: return "East";
		case Cardinal::West: return "West";
	}
}

bool Movement::IsStill() const
{
	return CurrMovement == MoveAction::None;
}

glm::vec3 Movement::DirOffset(Cardinal dir)
{
	switch (dir)
	{
		case Cardinal::North: return {  0,  0, -1 };
		case Cardinal::South: return {  0,  0,  1 };
		case Cardinal::East:  return {  1,  0,  0 };
		case Cardinal::West:  return { -1,  0,  0 };
	}

	return { 0,0,0 };
}

glm::vec3 Movement::GetNextTile(MoveAction action)
{
	glm::vec3 forward = DirOffset(CurrentDirection);
	glm::vec3 right = { -forward.z, 0, forward.x };
	glm::vec3 left = { forward.z, 0, -forward.x };

	if (action == MoveAction::Forwards)
		return Camera.CameraPos + forward;
	if (action == MoveAction::Backwards)
		return Camera.CameraPos - forward;
	if (action == MoveAction::Left)
		return Camera.CameraPos + left;
	if (action == MoveAction::Right)
		return Camera.CameraPos + right;
	else
		return glm::vec3{};
}

glm::vec3 Movement::GetNextEnemyTile(MoveAction action)
{
	glm::vec3 forward = DirOffset(CurrentDirection);
	glm::vec3 right = { -forward.z, 0, forward.x };
	glm::vec3 left = { forward.z, 0, -forward.x };

	if (action == MoveAction::Forwards)
		return (Camera.CameraPos + forward) + forward;
	if (action == MoveAction::Backwards)
		return Camera.CameraPos - forward - forward;
	if (action == MoveAction::Left)
		return Camera.CameraPos + left + left;
	if (action == MoveAction::Right)
		return Camera.CameraPos + right + right;
	else
		return glm::vec3{};
}

void Movement::MoveChar(float DeltaTime)
{
    if (CurrMovement == MoveAction::None) 
		return;

	glm::vec3 moveDir(0.0f);  // initialize to zero

	switch (CurrMovement) {
	case MoveAction::Forwards:
		moveDir = Camera.CameraFront;
		break;
	case MoveAction::Backwards:
		moveDir = -Camera.CameraFront;
		break;
	case MoveAction::Left:
		moveDir = -glm::normalize(glm::cross(Camera.CameraFront, Camera.CameraUp));
		break;
	case MoveAction::Right:
		moveDir = glm::normalize(glm::cross(Camera.CameraFront, Camera.CameraUp));
		break;
	default:
		moveDir = glm::vec3(0.0f);
	}

	if (glm::length(moveDir) > 0.0f) 
	{
		float moveAmount = MovementSpeed * DeltaTime;
		Camera.CameraPos += moveDir * moveAmount;
		DistanceMoved += moveAmount;

		if (DistanceMoved > MovementUnit)
		{
			DistanceMoved = 0.0f;
			Camera.CameraPos.x = floor(Camera.CameraPos.x + 0.5);
			Camera.CameraPos.z = floor(Camera.CameraPos.z + 0.5);

			EndMovement();

			//TODO: Move this into it's own folder one day
			if (G_Args.IsLiveEdit)
			{
				std::ofstream outfile("data\\pos.txt");
				outfile << Camera.CameraPos.x << ' ' << Camera.CameraPos.z;
				outfile.close();
			}
		}
	}
    else if (CurrMovement == MoveAction::TurnRight || CurrMovement == MoveAction::AutoTurnRight)
    {
        Camera.HorRot += RotationSpeed * DeltaTime;

		if (Camera.HorRot > (int)CurrentDirection + 90)
		{
            Camera.HorRot = (int)GetNextRightDir();
			CurrentDirection = GetNextRightDir();
			EndTurnMovement();
		}
    }
    else if (CurrMovement == MoveAction::TurnLeft || CurrMovement == MoveAction::AutoTurnLeft)
    {
        Camera.HorRot -= RotationSpeed * DeltaTime;

		if (Camera.HorRot < (int)CurrentDirection - 90)
		{
            Camera.HorRot = (int)GetNextLeftDir();
			CurrentDirection = GetNextLeftDir();
			EndTurnMovement();
		}
    }
    else if (CurrMovement == MoveAction::TurnAround || CurrMovement == MoveAction::AutoTurnAround)
    {
        Camera.HorRot -= RotationSpeed * DeltaTime;

		if (Camera.HorRot < (int)CurrentDirection - 180)
		{
            Camera.HorRot = (int)GetOppositeDir();
			CurrentDirection = GetOppositeDir();
			EndTurnMovement();
		}
    }
}

bool Movement::IsAutoMove(MoveAction ma)
{
	return (ma == MoveAction::AutoTurnAround ||
		    ma == MoveAction::AutoTurnRight  ||
		    ma == MoveAction::AutoTurnLeft);
}

void Movement::EndTurnMovement()
{
	if (IsAutoMove(CurrMovement))
	{
		if (BattleMessage->CurrentPhase == BattlePhase::Sighting)
		{
			BattleMessage->SetBattlePhase(BattlePhase::Preamble);
		}
	}

	CurrMovement = MoveAction::None;
}

void Movement::EndMovement()
{
	if (BattleMessage->CurrentPhase == BattlePhase::Sighting)
	{
		if (NextMove != MoveAction::None)
		{
			auto move = NextMove;
			NextMove = MoveAction::None;
			CurrMovement = MoveAction::None;
			SetMoveAction(move);
			return;
		}
		else
		{
			Tile* enemyTile = Map.GetTile(GetNextTile(CurrMovement));
			if (!enemyTile || enemyTile->IsWalkable)
			{
				BattleMessage->SetBattlePhase(BattlePhase::Preamble);
			}
		}
	}
	else
	{
		BattleMessage->DecreaseEnemyCounter();
	}

	CurrMovement = MoveAction::None;
}
