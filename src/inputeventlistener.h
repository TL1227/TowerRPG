#ifndef INPUTEVENTLISTENER_H
#define INPUTEVENTLISTENER_H

#include "inputaction.h"

class InputEventListener
{
public:
    virtual void OnButtonPress(InputAction button) = 0;
    virtual ~InputEventListener() = default;
};
#endif
