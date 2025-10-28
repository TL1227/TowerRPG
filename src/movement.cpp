#include <iostream>

#include "movement.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

void Movement::MoveChar(glm::vec3 *CameraPos, glm::vec3 CameraFront, float DeltaTime, float *HorRot)
{
    if (CurrMovement == MoveAction::Forwards)
    {
		*CameraPos += (MOVESPEED * DeltaTime) * CameraFront;
        MOVESTART += (MOVESPEED * DeltaTime);

        if (MOVESTART > MOVEDIST)
        {
            CurrMovement = MoveAction::None;
            MOVESTART = 0.0f;
            CameraPos->x = floor(CameraPos->x + 0.5);
            CameraPos->z = floor(CameraPos->z + 0.5);

            std::cout << "x: " << CameraPos->x << " z: " << CameraPos->z << std::endl;
        }
    }
    else if (CurrMovement == MoveAction::Backwards)
    {
		CameraPos -= (MOVESPEED * DeltaTime) * CameraFront;
        MOVESTART += (MOVESPEED * DeltaTime);

        if (MOVESTART > MOVEDIST)
        {
            CurrMovement = MoveAction::None;
            MOVESTART = 0.0f;
            CameraPos->x = floor(CameraPos->x + 0.5);
            CameraPos->z = floor(CameraPos->z + 0.5);
        }
    }
    else if (CurrMovement == MoveAction::TurnRight)
    {
        HorRot += ROTSPEED * DeltaTime;

		if (HorRot > CharMove.GetCurrent() + 90)
		{
			CurrMovement = MoveAction::None;
            HorRot = CharMove.GetNextRightDir();
            CharMove.SetCurrent(CharMove.GetNextRightDir());

			cout << HorRot << endl;
		}
    }
    else if (CurrMovement == MoveAction::TurnLeft)
    {
        HorRot -= ROTSPEED * DeltaTime;

		if (HorRot < CharMove.GetCurrent() - 90)
		{
			CurrMovement = MoveAction::None;
            HorRot = CharMove.GetNextLeftDir();
            CharMove.SetCurrent(CharMove.GetNextLeftDir());

			cout << HorRot << endl;
		}
    }
}
