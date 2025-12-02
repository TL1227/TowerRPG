#include <iostream>
#include <fstream>
#include <random>

#include "movementsystem.h"
#include "args.h"

static MoveAction NextMove = MoveAction::None;

MovementSystem::MovementSystem(::Map& map, ::Camera& camera)
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

Cardinal MovementSystem::GetNextRightDir() const
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

Cardinal MovementSystem::GetNextLeftDir() const
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

Cardinal MovementSystem::GetOppositeDir() const
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

void MovementSystem::SetCurrentMoveAction(MoveAction ma)
{
	CurrentMoveAction = ma;
	Event->DispatchMoveActionChange(ma);
}

void MovementSystem::SetCurrentDirection(Cardinal dr)
{
	CurrentDirection = dr;
	Event->DispatchDirectionChange(dr);
}

void MovementSystem::SetMoveDistance(float d)
{
	DistanceMoved = d;
	Event->DispatchMoveDistanceChange(d);
}

void MovementSystem::ProcessMoveAction(MoveAction action)
{
	if (CurrentMoveAction == MoveAction::None)
	{
		if (action == MoveAction::TurnRight ||
			action == MoveAction::TurnLeft ||
			action == MoveAction::TurnAround ||
			action == MoveAction::AutoTurnRight ||
			action == MoveAction::AutoTurnLeft ||
			action == MoveAction::AutoTurnAround)
		{
			SetCurrentMoveAction(action);
			return;
		}

		Tile* nextTile = nullptr;

		nextTile = Map.GetTile(GetNextTile(action));
		if (!nextTile || nextTile->IsWalkable)
		{
			Tile* enemyTile = Map.GetTile(GetNextEnemyTile(action));
			if (!enemyTile || enemyTile->IsWalkable)
			{
				if (BattleSystem->GetPhase() == BattlePhase::Sighting)
				{
					MovementSpeed = PreBattleMovementSpeed;
					Enemy->Position = GetNextEnemyTile(action);

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

			SetCurrentMoveAction(action);
		}
	}
}

std::string MovementSystem::PrintCurrentDirection()
{
	switch (CurrentDirection)
	{
		case Cardinal::North: return "North";
		case Cardinal::South: return "South";
		case Cardinal::East: return "East";
		case Cardinal::West: return "West";
	}
}

bool MovementSystem::IsStill() const
{
	return CurrentMoveAction == MoveAction::None;
}

glm::vec3 MovementSystem::DirOffset(Cardinal dir)
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

glm::vec3 MovementSystem::GetNextTile(MoveAction action)
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

glm::vec3 MovementSystem::GetNextEnemyTile(MoveAction action)
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

void MovementSystem::Tick(float DeltaTime)
{
    if (CurrentMoveAction == MoveAction::None) 
		return;

	glm::vec3 moveDir(0.0f);  // initialize to zero

	switch (CurrentMoveAction) {
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
		SetMoveDistance(DistanceMoved + moveAmount);

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
    else if (CurrentMoveAction == MoveAction::TurnRight || CurrentMoveAction == MoveAction::AutoTurnRight)
    {
        Camera.HorRot += RotationSpeed * DeltaTime;

		if (Camera.HorRot > (int)CurrentDirection + 90)
		{
            Camera.HorRot = (int)GetNextRightDir();
			SetCurrentDirection(GetNextRightDir());
			EndTurnMovement();
		}
    }
    else if (CurrentMoveAction == MoveAction::TurnLeft || CurrentMoveAction == MoveAction::AutoTurnLeft)
    {
        Camera.HorRot -= RotationSpeed * DeltaTime;

		if (Camera.HorRot < (int)CurrentDirection - 90)
		{
            Camera.HorRot = (int)GetNextLeftDir();
			SetCurrentDirection(GetNextLeftDir());
			EndTurnMovement();
		}
    }
    else if (CurrentMoveAction == MoveAction::TurnAround || CurrentMoveAction == MoveAction::AutoTurnAround)
    {
        Camera.HorRot -= RotationSpeed * DeltaTime;

		if (Camera.HorRot < (int)CurrentDirection - 180)
		{
            Camera.HorRot = (int)GetOppositeDir();
			SetCurrentDirection(GetOppositeDir());
			EndTurnMovement();
		}
    }
}

bool MovementSystem::IsAutoMove(MoveAction ma)
{
	return (ma == MoveAction::AutoTurnAround ||
		    ma == MoveAction::AutoTurnRight  ||
		    ma == MoveAction::AutoTurnLeft);
}

void MovementSystem::EndTurnMovement()
{
	if (IsAutoMove(CurrentMoveAction))
	{
		//TODO: movement finished event
		BattleSystem->AutoMoveFinished();
	}

	SetCurrentMoveAction(MoveAction::None);
}

void MovementSystem::EndMovement()
{
	if (BattleSystem->GetPhase() == BattlePhase::Sighting)
	{
		if (NextMove != MoveAction::None)
		{
			MoveAction move = NextMove;
			NextMove = MoveAction::None;
			SetCurrentMoveAction(MoveAction::None);
			ProcessMoveAction(move);
			return;
		}
		else
		{
			Tile* enemyTile = Map.GetTile(GetNextTile(CurrentMoveAction));
			if (!enemyTile || enemyTile->IsWalkable)
			{
				BattleSystem->SetBattlePhase(BattlePhase::Preamble);
			}
		}
	}
	else
	{
		BattleSystem->DecreaseEnemyCounter();
	}

	SetCurrentMoveAction(MoveAction::None);
}

void MovementSystem::OnBattlePhaseChange(BattlePhase bp)
{
	switch (bp)
	{
	case BattlePhase::Sighting: break;
	case BattlePhase::Preamble: break;
	case BattlePhase::Slide: break;
	case BattlePhase::Snap: break;
	case BattlePhase::Start: break;
	case BattlePhase::End: { MovementSpeed = NormalMovementSpeed; } break;
	default:
		break;
	}
}
