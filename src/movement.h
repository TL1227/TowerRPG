#ifndef MOVEMENT_H
#define MOVEMENT_H

enum class MoveAction
{
	None,
	Forwards,
	Backwards,
	TurnRight,
	TurnLeft
};

class Movement
{
public:
    int GetNextRightDir();
    int GetNextLeftDir();
	void SetMoveAction(MoveAction);
	void MoveChar();
    int GetCurrent() { return CurrentDirection; }
    void SetCurrent(float direction) { CurrentDirection = direction; }


private:
	int North = 90;
	int East = 180;
	int South = 270;
	int West = 0;
    int CurrentDirection;

	float MOVESTART = 0.0f;
	float MOVEDIST = 1.0f;
	float MOVESPEED = 2.0f;
	float ROTSTART = 0.0f;
	float ROTDIST = 90.0f;
	float ROTSPEED = 130.0f;

	MoveAction CurrMovement = MoveAction::None;
};

#endif
