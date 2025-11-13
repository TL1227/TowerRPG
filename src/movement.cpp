#include <iostream>
#include <fstream>

#include "movement.h"
#include "args.h"

int Movement::GetNextRightDir()
{
	if (CurrentDirection == North)
		return East;
	if (CurrentDirection == East)
		return South;
	if (CurrentDirection == South)
		return West;
	if (CurrentDirection == West)
		return North;
}

int Movement::GetNextLeftDir()
{
	if (CurrentDirection == North)
		return West;
	if (CurrentDirection == West)
		return South;
	if (CurrentDirection == South)
		return East;
	if (CurrentDirection == East)
		return North;
}

void Movement::SetMoveAction(MoveAction action)
{
	if (CurrMovement == MoveAction::None)
	{
		CurrMovement = action;
	}
}

void Movement::MoveChar(Camera &camera, float DeltaTime)
{
    if (CurrMovement == MoveAction::None) 
		return;

	glm::vec3 moveDir(0.0f);  // initialize to zero

	switch (CurrMovement) {
	case MoveAction::Forwards:
		moveDir = camera.CameraFront;
		break;
	case MoveAction::Backwards:
		moveDir = -camera.CameraFront;
		break;
	case MoveAction::Left:
		moveDir = -glm::normalize(glm::cross(camera.CameraFront, camera.CameraUp));
		break;
	case MoveAction::Right:
		moveDir = glm::normalize(glm::cross(camera.CameraFront, camera.CameraUp));
		break;
	default:
		moveDir = glm::vec3(0.0f);
	}

	if (glm::length(moveDir) > 0.0f) 
	{
		float moveAmount = MovementSpeed * DeltaTime;
		camera.CameraPos += moveDir * moveAmount;
		DistanceMoved += moveAmount;

		if (DistanceMoved > MovementUnit)
		{
			CurrMovement = MoveAction::None;
			DistanceMoved = 0.0f;
			camera.CameraPos.x = floor(camera.CameraPos.x + 0.5);
			camera.CameraPos.z = floor(camera.CameraPos.z + 0.5);

			//std::cout << "x: " << camera.CameraPos.x << ' '
				//<< "z: " << camera.CameraPos.z << std::endl;

			//TODO: Only call this if we're in LiveEdit
			if (G_Args.IsLiveEdit)
			{
				std::ofstream outfile("C:\\Users\\Tosh\\Projects\\Crimson Tower\\TowerRPG\\data\\pos.txt");
				outfile << camera.CameraPos.x << ' ' << camera.CameraPos.z;
				outfile.close();
			}
		}
	}
    else if (CurrMovement == MoveAction::TurnRight)
    {
        camera.HorRot += RotationSpeed * DeltaTime;

		if (camera.HorRot > GetCurrentDirection() + 90)
		{
			CurrMovement = MoveAction::None;
            camera.HorRot = GetNextRightDir();
            SetCurrentDirection(GetNextRightDir());
		}
    }
    else if (CurrMovement == MoveAction::TurnLeft)
    {
        camera.HorRot -= RotationSpeed * DeltaTime;

		if (camera.HorRot < GetCurrentDirection() - 90)
		{
			CurrMovement = MoveAction::None;
            camera.HorRot = GetNextLeftDir();
            SetCurrentDirection(GetNextLeftDir());
		}
    }
}
