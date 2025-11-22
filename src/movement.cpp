#include <iostream>
#include <fstream>
#include <random>

#include "movement.h"
#include "args.h"

//maybe do this?
//#define FORWARD (int)MoveAction::Forwards

static MoveAction NextMove = MoveAction::None;

Movement::Movement(::Map& map, ::Camera& camera)
	: Map{ map }, Camera{camera}
{
	SetSurroundingTiles();
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
			action == MoveAction::TurnAround) 
		{
			CurrMovement = action;
			return;
		}

		Tile* nextTile = nullptr;

		//note: a nullptr here means theres no tile, so an empty space
		//once I've created explicit ground tiles we can get rid of this check
		nextTile = Map.GetTile(SurroundingTiles[(int)action]);
		if (!nextTile || nextTile->IsWalkable)
		{
			Tile* enemyTile = Map.GetTile(EnemyTiles[(int)action]);
			if (!enemyTile || enemyTile->IsWalkable)
			{
				if (EnemyCounter <= 0)
				{
					MovementSpeed = PreBattleMovementSpeed;
					WeBattleNow = true;
					Enemy->Position = EnemyTiles[(int)action];
					Enemy->PlayerDirection = (float)CurrentDirection;
					EnemyCounter = RandomNumber();

					// turn to face the enemy
					if (action == MoveAction::Left)
					{
						NextMove = MoveAction::TurnLeft;
						Enemy->PlayerDirection = (float)GetNextLeftDir();
					}
					if (action == MoveAction::Right)
					{
						NextMove = MoveAction::TurnRight;
						Enemy->PlayerDirection = (float)GetNextRightDir();
					}
					if (action == MoveAction::Backwards)
					{
						NextMove = MoveAction::TurnAround;
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
	WeBattleNow = false;
	MovementSpeed = NormalMovementSpeed;
}

std::string Movement::PrintDir()
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

void Movement::SetSurroundingTiles()
{
	glm::vec3 forward = DirOffset(CurrentDirection);
	glm::vec3 right = { -forward.z, 0, forward.x };
	glm::vec3 left = { forward.z, 0, -forward.x };
	
	//note: these match the MoveAction enums
	SurroundingTiles[0] = Camera.CameraPos + forward;
	SurroundingTiles[1] = Camera.CameraPos - forward;
	SurroundingTiles[2] = Camera.CameraPos + left;
	SurroundingTiles[3] = Camera.CameraPos + right;

	EnemyTiles[0] = SurroundingTiles[0] + forward;
	EnemyTiles[1] = SurroundingTiles[1] - forward;
	EnemyTiles[2] = SurroundingTiles[2] + left;
	EnemyTiles[3] = SurroundingTiles[3] + right;

	FrontTile = Map.GetTile(SurroundingTiles[0]);
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
			CurrMovement = MoveAction::None;
			DistanceMoved = 0.0f;
			Camera.CameraPos.x = floor(Camera.CameraPos.x + 0.5);
			Camera.CameraPos.z = floor(Camera.CameraPos.z + 0.5);

			EndMovement();

			EnemyCounter--;
			cout << EnemyCounter << endl;

			//TODO: Move this into it's own folder one day
			if (G_Args.IsLiveEdit)
			{
				std::ofstream outfile("data\\pos.txt");
				outfile << Camera.CameraPos.x << ' ' << Camera.CameraPos.z;
				outfile.close();
			}
		}
	}
    else if (CurrMovement == MoveAction::TurnRight)
    {
        Camera.HorRot += RotationSpeed * DeltaTime;

		if (Camera.HorRot > (int)CurrentDirection + 90)
		{
			CurrMovement = MoveAction::None;
            Camera.HorRot = (int)GetNextRightDir();
			CurrentDirection = GetNextRightDir();
			EndMovement();
		}
    }
    else if (CurrMovement == MoveAction::TurnLeft)
    {
        Camera.HorRot -= RotationSpeed * DeltaTime;

		if (Camera.HorRot < (int)CurrentDirection - 90)
		{
			CurrMovement = MoveAction::None;
            Camera.HorRot = (int)GetNextLeftDir();
			CurrentDirection = GetNextLeftDir();
			EndMovement();
		}
    }
    else if (CurrMovement == MoveAction::TurnAround)
    {
        Camera.HorRot -= RotationSpeed * DeltaTime;

		if (Camera.HorRot < (int)CurrentDirection - 180)
		{
			CurrMovement = MoveAction::None;
            Camera.HorRot = (int)GetOppositeDir();
			CurrentDirection = GetOppositeDir();
			EndMovement();
		}
    }
}

float Movement::GetDirection()
{
	return (float)CurrentDirection;
}

void Movement::EndMovement()
{
	SetSurroundingTiles();

	if (NextMove != MoveAction::None)
	{
		auto move = NextMove;
		NextMove = MoveAction::None;
		SetMoveAction(move);
	}
}

