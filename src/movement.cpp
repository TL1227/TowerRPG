#include <iostream>
#include <fstream>

#include "movement.h"
#include "args.h"

Movement::Movement(::Map& map, ::Camera& camera)
	: Map{ map }, Camera{camera}
{
	SetSurroundingTiles();
}

Direction Movement::GetNextRightDir() const
{
	if (CurrentDirection == Direction::North)
		return Direction::East;
	if (CurrentDirection == Direction::East)
		return Direction::South;
	if (CurrentDirection == Direction::South)
		return Direction::West;
	if (CurrentDirection == Direction::West)
		return Direction::North;
}

Direction Movement::GetNextLeftDir() const
{
	if (CurrentDirection == Direction::North)
		return Direction::West;
	if (CurrentDirection == Direction::West)
		return Direction::South;
	if (CurrentDirection == Direction::South)
		return Direction::East;
	if (CurrentDirection == Direction::East)
		return Direction::North;
}

bool BlockIsSolid(char ch)
{
	return (ch == '#' || ch == 'c');
}

void Movement::SetMoveAction(MoveAction action)
{
	if (CurrMovement == MoveAction::None)
	{
		if (action == MoveAction::Forwards)
		{
			Tile* tp = Map.GetTile(FrontTilePos);

			if(!tp || tp->IsWalkable)
				CurrMovement = action;
		}
		else if (action == MoveAction::Backwards)
		{
			Tile* tp = Map.GetTile(BackTilePos);

			if(!tp || tp->IsWalkable)
				CurrMovement = action;
		}
		else if (action == MoveAction::Left)
		{
			Tile* tp = Map.GetTile(LeftTilePos);

			if(!tp || tp->IsWalkable)
				CurrMovement = action;
		}
		else if (action == MoveAction::Right)
		{
			Tile* tp = Map.GetTile(RightTilePos);

			if(!tp || tp->IsWalkable)
				CurrMovement = action;
		}
		else if (action == MoveAction::TurnRight || action == MoveAction::TurnLeft)
		{
			CurrMovement = action;
		}
	}
}

std::string Movement::PrintDir()
{
	switch (CurrentDirection)
	{
		case Direction::North: return "North";
		case Direction::South: return "South";
		case Direction::East: return "East";
		case Direction::West: return "West";
	}
}

bool Movement::IsStill() const
{
	return CurrMovement == MoveAction::None;
}

glm::vec3 Movement::DirOffset(Direction dir)
{
	switch (dir)
	{
		case Direction::North: return {  0,  0, -1 };
		case Direction::South: return {  0,  0,  1 };
		case Direction::East:  return {  1,  0,  0 };
		case Direction::West:  return { -1,  0,  0 };
	}

	return { 0,0,0 };
}

void Movement::SetSurroundingTiles()
{
	glm::vec3 forward = DirOffset(CurrentDirection);
	glm::vec3 right = { -forward.z, 0, forward.x };
	glm::vec3 left = { forward.z, 0, -forward.x };
	
	FrontTilePos = Camera.CameraPos + forward;
	BackTilePos = Camera.CameraPos - forward;
	LeftTilePos = Camera.CameraPos + left;
	RightTilePos = Camera.CameraPos + right;

	FrontTile = Map.GetTile(FrontTilePos);
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

			SetSurroundingTiles();

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
			SetSurroundingTiles();
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
			SetSurroundingTiles();
		}
    }

	std::cout << (int)CurrentDirection << std::endl;
}
