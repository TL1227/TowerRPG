#ifndef INPUTEVENTLISTENER_H
#define INPUTEVENTLISTENER_H

#include "inputaction.h"
#include "moveaction.h"
#include "menuaction.h"

class InputEventListener
{
public:
    virtual void OnButtonPress(InputAction button) {};
    virtual void OnMoveActionButtonPress(MoveAction button) {};
    virtual void OnMenuActionButtonPress(MenuAction button) {};
    virtual ~InputEventListener() = default;
};
#endif
