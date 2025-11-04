#include <iostream>

#include "movement.h"

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
    if (CurrMovement == MoveAction::Forwards)
    {
        camera.CameraPos += (MovementSpeed * DeltaTime) * camera.CameraFront;
        MOVESTART += (MovementSpeed * DeltaTime);

        if (MOVESTART > MOVEDIST)
        {
            CurrMovement = MoveAction::None;
            MOVESTART = 0.0f;
            camera.CameraPos.x = floor(camera.CameraPos.x + 0.5);
            camera.CameraPos.z = floor(camera.CameraPos.z + 0.5);

            std::cout << "x: " << camera.CameraPos.x << " z: " << camera.CameraPos.z << std::endl;
        }
    }
    else if (CurrMovement == MoveAction::Backwards)
    {
		camera.CameraPos -= (MovementSpeed * DeltaTime) * camera.CameraFront;
        MOVESTART += (MovementSpeed * DeltaTime);

        if (MOVESTART > MOVEDIST)
        {
            CurrMovement = MoveAction::None;
            MOVESTART = 0.0f;
            camera.CameraPos.x = floor(camera.CameraPos.x + 0.5);
            camera.CameraPos.z = floor(camera.CameraPos.z + 0.5);
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

			std::cout <<camera.HorRot << std::endl;
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

			std::cout <<camera.HorRot << std::endl;
		}
    }
}
