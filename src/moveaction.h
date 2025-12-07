#ifndef MOVEACTION_H
#define MOVEACTION_H

enum class MoveAction
{
    None,
    Forwards,
    Backwards,
    Left,
    Right,
    TurnLeft,
    TurnRight,
    TurnAround,

    //TODO: These actions aren't really inputs and should maybe be removed from this enum?
	AutoTurnRight,
	AutoTurnLeft,
	AutoTurnAround
};

#endif
