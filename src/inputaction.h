#ifndef MOVEACTION_H
#define MOVEACTION_H

enum class InputAction
{
    None,
    Confim,
    Cancel,
    Up,
    Down,
    Left,
    Right,
    MoveForwards,
    MoveBackwards,
    MoveLeft,
    MoveRight,
    TurnLeft,
    TurnRight,
    TurnAround,
    MenuUp,
    MenuDown,
    MenuRight,
    MenuLeft,

    //TODO: These actions aren't really inputs and should maybe be removed from this enum?
	AutoTurnRight,
	AutoTurnLeft,
	AutoTurnAround
};

#endif
